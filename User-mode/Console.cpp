#include "Console.h"

#pragma warning(disable : 4996) // <------

Console::Console(const wchar_t *consoleTitle):title(consoleTitle)
{

	if (AllocConsole()) {

		freopen("CONOUT$", "wt", stdout);
		SetConsoleTitle(title);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		//ShowCursor(FALSE);
	}
	else
		throw ConsoleCreationException();

}

Console::~Console()
{

	FreeConsole();

}
