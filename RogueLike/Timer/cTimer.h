#pragma once
#include <chrono>
#include "timerType.h"

class cTimer
{
protected:
    steadyTimer::time_point   m_lastTime;     //마지막에 타이머를 확인한 시점
public:
    cTimer();
    ~cTimer();
public:
    deltaTimeType Update();         //lastTime을 갱신하고 lastTime에서 지난시간
    deltaTimeType ElapsedTime();    //타이머가 시작하고 현재까지의 경과 시간
    steadyTimer::time_point LastTime() { return m_lastTime; }
};

