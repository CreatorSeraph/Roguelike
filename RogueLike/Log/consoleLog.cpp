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

    std::locale loc("kor");
    auto lastLocOut = std::wcout.imbue(loc);
    auto lastLocLog = std::wclog.imbue(loc);
    auto lastLocErr = std::wcerr.imbue(loc);

    std::ios::sync_with_stdio(true);
}

consoleLog::~consoleLog()
{
    FreeConsole();
}
