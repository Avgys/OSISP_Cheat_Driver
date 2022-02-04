#pragma once
#include <ntddk.h>
extern "C" {

	NTSTATUS Open(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
	NTSTATUS Close(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
	NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject, _In_ PUNICODE_STRING pRegistryPath);
	VOID DriverUnload(_In_ PDRIVER_OBJECT pDriverObject);
	NTSTATUS DeviceControl(_In_ PDEVICE_OBJECT pDeviceObject, _In_ PIRP Irp);
	NTSTATUS WriteToLogFile(const CHAR* data, ULONG size);
}	