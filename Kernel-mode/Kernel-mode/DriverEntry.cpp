#include <ntifs.h>
#include <ntddk.h>
#include <ntdef.h>
#include "Msg.hpp"
#include "Driver.hpp"
#include <ntstrsafe.h>
#include <Ntstrsafe.h>


#define USER_MEMORY_SIZE 7516192768ULL
#define OFFSET 65536ULL
#define MI_POOL_COPY_BYTES    512
#define DRIVER_DEVICE L"cheatMoneyDevice"
#define START_DRIVER 134225923UL//CTL_CODE(DEVICE, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)
#define STOP_DRIVER 134225927UL
#define FIND_VALUE 134225931UL
#define FILTER_VALUE 134225935UL
#define SET_VALUE 134225939UL
#define ADDRS_SIZE 1024UL

extern "C" {	 
	NTSTATUS WriteToLogFile(const CHAR* data, ULONG size);
		
	NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject, _In_ PUNICODE_STRING pRegistryPath) {

		NTSTATUS status = STATUS_SUCCESS;
		//Driver load calls at the level IRGL_PASSIVE_LEVEL. 
		DbgPrint("Service registry path: %wZ", pRegistryPath);
		DbgPrint("SERVICE ENTRY gigig");
		//Register driver unload function.
		pDriverObject->DriverUnload = DriverUnload;
		//Register close/open functions for logic file, for communication with user-mode.
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = Open;
		pDriverObject->MajorFunction[IRP_MJ_CLOSE] = Close;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControl;
		//Virtual device creating.
		UNICODE_STRING deviceName = RTL_CONSTANT_STRING(L"\\Device\\cheatMoneyDevice");
		PDEVICE_OBJECT pDeviceObject = NULL;
		status = IoCreateDevice(pDriverObject,
			0, &deviceName, FILE_DEVICE_UNKNOWN,
			0, TRUE, &pDeviceObject);
		if (!NT_SUCCESS(status)) {
			DbgPrint("Creating device failed.");
			return status;
		}
		//Creating symbol link.
		UNICODE_STRING symbolLink = RTL_CONSTANT_STRING(L"\\??\\cheatMoneyDevice");
		status = IoCreateSymbolicLink(&symbolLink, &deviceName);
		if (!NT_SUCCESS(status)) {
			DbgPrint("Creating symbol link failed.");
			IoDeleteDevice(pDeviceObject);
			return status;
		}
		DbgPrint("Driver loaded successfuly.");

		return STATUS_SUCCESS;
	}

	NTSTATUS WriteMemory(PVOID Destination, PVOID Buffer, SIZE_T BufferSize, ULONG fProtect, KPROCESSOR_MODE ProcessorMode) // Write memory
	{
		PMDL mdl = IoAllocateMdl(Destination, (ULONG)BufferSize, FALSE, FALSE, NULL); // Allocate Memory Descriptor
		// Many MDL functions must be enclosed in a try/except statement
		__try
		{
			MmProbeAndLockPages(mdl, ProcessorMode, IoReadAccess);
			Destination = MmGetSystemAddressForMdlSafe(mdl, HighPagePriority);

			MmProtectMdlSystemAddress(mdl, PAGE_EXECUTE_READWRITE); // Set the page rights to R/W/X
			RtlCopyMemory(Destination, Buffer, sizeof(DWORD32)); // Write Memory
			MmProtectMdlSystemAddress(mdl, fProtect); // Set back to old page rights

			MmUnmapLockedPages(Destination, mdl);
			MmUnlockPages(mdl);
			IoFreeMdl(mdl); // free MDL
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return 1;
		}

		return 0;
	}

	void ReadMemory(PVOID Buffer, PVOID MemoryToRead, SIZE_T Size)
	{
		RtlCopyMemory(Buffer, MemoryToRead, Size);
	}

	NTSTATUS DeviceControl(_In_ PDEVICE_OBJECT pDeviceObject, _In_ PIRP Irp) {
		UNREFERENCED_PARAMETER(pDeviceObject);
		NTSTATUS status = STATUS_SUCCESS;
		PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
		//This value identifies the callback routine.
		static LARGE_INTEGER cookie = { 0 };
		static size_t addrs[ADDRS_SIZE] = { 0 };
		const ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;
		switch (controlCode) {
		case START_DRIVER: {
			if (!NT_SUCCESS(status)) {
				DbgPrint("Registration register callback error.");
			}
			else {
				DbgPrint("Driver started...");
			}
			break;
		}
		case FIND_VALUE: {
			if (KeGetCurrentIrql() == PASSIVE_LEVEL) {
				Msg* msg = (Msg*)stack->Parameters.DeviceIoControl.Type3InputBuffer;
				PEPROCESS pPr;
				DbgPrint("FIND VALUE.");
				HANDLE id = PsGetCurrentProcessId();
				DbgPrint("Curr id %llu.", (size_t)id);
				
				status = PsLookupProcessByProcessId((HANDLE)msg->pid, &pPr);

				if (NT_SUCCESS(status)) {
					DbgPrint("Found %llu.", msg->pid);
					DbgPrint("Found %p.", (HANDLE)msg->pid);
				}
				else {
					DbgPrint("Not found procces pid%llu.", msg->pid);
					break;
				}
				KAPC_STATE ApcState = { 0 };
				char buf[1024] = { 0 };
				size_t bufLength = msg->valueLength;
				memcpy(buf, msg->value, msg->valueLength);
				memset(addrs, 0, sizeof(addrs));

				KeStackAttachProcess(pPr, &ApcState);
				DbgPrint("Called KeStackAttachProcess");

				WriteToLogFile("Attached to process\n", 21);
				id = PsGetProcessId(pPr);
				DbgPrint("Curr id %llu.", (size_t)id);


				OBJECT_ATTRIBUTES objAttribs;
				CLIENT_ID clientId;

				clientId.UniqueThread = 0;
				clientId.UniqueProcess = id;

				InitializeObjectAttributes(&objAttribs,
					NULL,
					OBJ_KERNEL_HANDLE,
					NULL,
					NULL);

				HANDLE hProcess = NULL;

				status = ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &objAttribs, &clientId);
				DbgPrint("Called ZwOpenProcess");
				if (!NT_SUCCESS(status)) {

					DbgPrint("Open process failed");
					break;
				}			

				size_t length = 0;
				size_t index = 0;
				size_t start = OFFSET;
				size_t end = (size_t)-1;
				MEMORY_BASIC_INFORMATION mbi = { 0 };
				for (size_t memptr = start; memptr < end; memptr = (size_t)mbi.BaseAddress + mbi.RegionSize)
				{
					status = ZwQueryVirtualMemory(hProcess, (PVOID)memptr, MemoryBasicInformation, &mbi, sizeof(mbi), &length);
					WriteToLogFile("ZwQueryVirtualMemory\n",22);
					if (!NT_SUCCESS(status))
					{
						DbgPrint("BlackBone: %s: ZwQueryVirtualMemory for address 0x%llu returned status 0x%X\n", __FUNCTION__, memptr, status);
						break;
					}
					// Skip non-committed, no-access and guard pages
					else if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS && !(mbi.Protect & PAGE_GUARD))
					{
						DbgPrint("Called ZwQueryVirtualMemory baseAddr:%llu, RegionSize:%llu", (size_t)mbi.BaseAddress, mbi.RegionSize);
						for (size_t j = memptr; j < memptr + mbi.RegionSize - bufLength; j++) {
							if (NT_SUCCESS(status)) {
								if (memcmp(buf, (char*)j, bufLength) == 0) {
									if (index < ADDRS_SIZE) {
										addrs[index++] = j;
										DbgPrint("Success read memory: %llu", addrs[index - 1]);
										WriteToLogFile("Success read memory\n", 21);
									}
									else {
										DbgPrint("Not enough address array: %llu", addrs[index - 1]);
										WriteToLogFile("Not enough address array\n", 26);
										break;
									}
								}
							}
							else
							{
								DbgPrint("Error read memory:%llu : %llu", j, (size_t)status);
							}
						}
					}
				}
				DbgPrint("Ended reading memory at %llu, with end %llu", (size_t)mbi.BaseAddress + mbi.RegionSize, end);

				if (hProcess != NULL) {
					ZwClose(hProcess);
				}

				KeUnstackDetachProcess(&ApcState);
				ObDereferenceObject(pPr);
				status = STATUS_SUCCESS;
				for (size_t i = 0; i < sizeof(addrs) / sizeof(size_t); i++) {
					msg->addrs[i] = addrs[i];
					DbgPrint("Saved address %llu", msg->addrs[i]);
				}
				DbgPrint("Dettached to proccess.");
				WriteToLogFile("Dettached read memory\n", 23);
				break;
			}
			else 
			{
				status = IRQL_NOT_LESS_OR_EQUAL;
				break;
			}
		}
		case FILTER_VALUE: {
			if (KeGetCurrentIrql() <= APC_LEVEL) {
				Msg* msg = (Msg*)stack->Parameters.DeviceIoControl.Type3InputBuffer;
				PEPROCESS pPr;
				DbgPrint("FIND VALUE.");
				HANDLE id = PsGetCurrentProcessId();
				DbgPrint("Curr id %llu.", (size_t)id);
				status = PsLookupProcessByProcessId((HANDLE)msg->pid, &pPr);

				if (NT_SUCCESS(status)) {
					DbgPrint("Found %llu.", msg->pid);
					DbgPrint("Found %p.", (HANDLE)msg->pid);
				}
				else {
					DbgPrint("Not found procces pid%llu.", msg->pid);
				}
				KAPC_STATE ApcState = { 0 };
				char buf[1024] = { 0 };
				size_t bufLength = msg->valueLength;
				memcpy(buf, msg->value, msg->valueLength);

				size_t oldAddrs[ADDRS_SIZE] = { 0 };

				size_t index = 0;
				memcpy(oldAddrs, addrs, sizeof(addrs));
				memset(addrs, 0, sizeof(addrs));
				KeStackAttachProcess(pPr, &ApcState);
				DbgPrint("Called KeStackAttachProcess");
				id = PsGetProcessId(pPr);
				DbgPrint("Attached to process");
				DbgPrint("Curr id %llu.", (size_t)id);

				for (size_t i = 0; i < sizeof(addrs) / sizeof(size_t); i++)
				{
					if (oldAddrs[i] != 0)
						if (memcmp(buf, (char*)oldAddrs[i], bufLength) == 0) {
							if (index < ADDRS_SIZE) {
								addrs[index++] = oldAddrs[i];
								DbgPrint("Success read filter: %llu", addrs[index - 1]);
								WriteToLogFile("Success read filter\n", 21);
							}
							else {
								DbgPrint("Not enough address array: %llu", addrs[index - 1]);
								WriteToLogFile("Not enough address array\n", 26);
								break;
							}
						}
				}

				KeUnstackDetachProcess(&ApcState);
				ObDereferenceObject(pPr);
				status = STATUS_SUCCESS;
				for (size_t i = 0; i < sizeof(addrs) / sizeof(size_t); i++) {
					msg->addrs[i] = addrs[i];
					DbgPrint("Saved filtered address %llu", msg->addrs[i]);
				}
				DbgPrint("Dettached to proccess.");
				break;
			}
			else {
				status = IRQL_NOT_LESS_OR_EQUAL;
				break;
			}
		}
		case SET_VALUE: {
			if(KeGetCurrentIrql() <= APC_LEVEL){
				Msg* msg = (Msg*)stack->Parameters.DeviceIoControl.Type3InputBuffer;
				PEPROCESS pPr;
				DbgPrint("SET VALUE.");
				HANDLE id = PsGetCurrentProcessId();
				DbgPrint("Curr id %llu.", (size_t)id);
				status = PsLookupProcessByProcessId((HANDLE)msg->pid, &pPr);

				if (NT_SUCCESS(status)) {
					DbgPrint("Found %llu.", msg->pid);
					DbgPrint("Found %p.", (HANDLE)msg->pid);
				}
				else {
					DbgPrint("Not found procces pid%llu.", msg->pid);
				}
				KAPC_STATE ApcState = { 0 };
				char buf[1024] = { 0 };
				size_t bufLength = msg->valueLength;
				memcpy(buf, msg->value, msg->valueLength);
				void* AddrToSet = msg->pFound;

				KeStackAttachProcess(pPr, &ApcState);
				id = PsGetProcessId(pPr);
				DbgPrint("Attached to process");
				DbgPrint("Curr id %llu.", (size_t)id);


				__try {
					status = WriteMemory(AddrToSet, buf, bufLength, PAGE_READONLY, UserMode);
					if (NT_SUCCESS(status)) {
						DbgPrint("Wrote to address success");
					}
					else {
						DbgPrint("Not wrote to pid.");
					}
				}
				__except (STATUS_ACCESS_VIOLATION) {
					DWORD error = GetExceptionCode();
					DbgPrint("readVal id %lu.", (DWORD)error);
				}
				KeUnstackDetachProcess(&ApcState);
				ObDereferenceObject(pPr);
				status = STATUS_SUCCESS;
				break;
			}
			else
			{
			status = IRQL_NOT_LESS_OR_EQUAL;
			break;
			}
		}
		case STOP_DRIVER: {
			if (!NT_SUCCESS(status)) {
				DbgPrint("Unregistration failure...");
			}
			else {
				DbgPrint("Driver stopped...");
			}
			break;
		}
		default: {
			status = STATUS_INVALID_DEVICE_REQUEST;
			break;
			}
		}
		Irp->IoStatus.Status = status;
		Irp->IoStatus.Information = 0;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return status;
	}

	NTSTATUS Open(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp) {

		UNREFERENCED_PARAMETER(DeviceObject);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return STATUS_SUCCESS;
	}

	NTSTATUS Close(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp) {

		UNREFERENCED_PARAMETER(DeviceObject);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return STATUS_SUCCESS;
	}

	VOID DriverUnload(_In_ PDRIVER_OBJECT pDriverObject) {

		UNREFERENCED_PARAMETER(pDriverObject);
		NTSTATUS status = STATUS_SUCCESS;
		//Delete symbol link.
		UNICODE_STRING symbolLink = RTL_CONSTANT_STRING(L"\\??\\cheatMoneyDevice");
		status = IoDeleteSymbolicLink(&symbolLink);
		if (!NT_SUCCESS(status)) {
			DbgPrint("Deleting symbol link error.");
		}
		//Delete device.
		IoDeleteDevice(pDriverObject->DeviceObject);
		DbgPrint("Driver unloaded successfuly.");
	}


	NTSTATUS WriteToLogFile(const CHAR* data, ULONG size) {

		UNICODE_STRING     uniName;
		RtlInitUnicodeString(&uniName, L"\\SystemRoot\\RegistryLog.txt");

		OBJECT_ATTRIBUTES  objAttr;
		InitializeObjectAttributes(&objAttr, &uniName,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
			NULL, NULL);

		HANDLE handle = NULL;
		IO_STATUS_BLOCK    ioStatusBlock;
		NTSTATUS ntstatus = ZwCreateFile(&handle,
			FILE_GENERIC_WRITE,
			&objAttr, &ioStatusBlock,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			0,
			FILE_OPEN_IF,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL, 0);
		if (NT_SUCCESS(ntstatus)) {
			LARGE_INTEGER ByteOffset;
			ByteOffset.HighPart = -1;
			ByteOffset.LowPart = FILE_WRITE_TO_END_OF_FILE;
			ntstatus = ZwWriteFile(handle, NULL, NULL, NULL, &ioStatusBlock,
				(PVOID)data, size, &ByteOffset, NULL);
			KdPrint((data));
			ntstatus = ZwClose(handle);
		}

		return ntstatus;
	}
}
