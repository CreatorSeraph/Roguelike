#pragma once
#include <chrono>
#include "timerType.h"

class cTimer
{
protected:
    steadyTimer::time_point   m_lastTime;
public:
    cTimer();
    ~cTimer();
public:
    //lastTime을 갱신하고 lastTime에서 지난시간
    deltaTimeType Update();
    //마지막 프레임 체크로부터 경과 시간
    deltaTimeType ElapsedTime();
    steadyTimer::time_point LastTime() { return m_lastTime; }
};

