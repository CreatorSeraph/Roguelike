#pragma once
#include <fstream>

class consoleLog
{
protected:
    std::wofstream m_logFile;
public:
    consoleLog();
    ~consoleLog();
};