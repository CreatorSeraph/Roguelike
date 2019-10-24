#include "pch.h"
#include "consoleLog.h"

#include <locale>

consoleLog::consoleLog()
{
    AllocConsole();

    FILE* fpTemp;
    fpTemp = freopen("CONOUT$", "w", stdout);
    fpTemp = freopen("CONOUT$", "w", stderr);
    fpTemp = freopen("CONIN$", "r", stdin);

    auto locale = std::wcout.imbue(std::locale("kor"));
}

consoleLog::~consoleLog()
{
    FreeConsole();
}
