#include "pch.h"
#include "consoleLog.h"

consoleLog::consoleLog()
{
    AllocConsole();

    FILE* fpTemp;
    fpTemp = freopen("CONOUT$", "w", stdout);
    fpTemp = freopen("CONOUT$", "w", stderr);
    fpTemp = freopen("CONIN$", "r", stdin);
}

consoleLog::~consoleLog()
{
    FreeConsole();
}
