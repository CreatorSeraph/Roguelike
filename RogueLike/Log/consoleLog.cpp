#include "pch.h"
#include "consoleLog.h"

#include <locale>

consoleLog::consoleLog()
    : m_logFile(L"logFile.txt")
{
    AllocConsole();

    FILE* fpTemp;
    fpTemp = freopen("CONOUT$", "w", stdout);
    fpTemp = freopen("CONOUT$", "w", stderr);
    fpTemp = freopen("CONIN$", "r", stdin);

    std::locale loc("kor");
    auto lastLocOut = wcout.imbue(loc);
    auto lastLocLog = wclog.imbue(loc);
    auto lastLocErr = wcerr.imbue(loc);

    auto fileBuf = m_logFile.rdbuf();
    auto lastBuf = wclog.rdbuf(fileBuf);

    std::ios::sync_with_stdio(true);
}

consoleLog::~consoleLog()
{
    FreeConsole();
}
