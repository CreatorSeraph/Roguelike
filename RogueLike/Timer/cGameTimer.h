#pragma once
#include <chrono>
#include "timerType.h"
#include "cTimer.h"

class cGameTimer
{
public:
    MAKE_SINGLETONFUNC(cGameTimer, GetInst)
protected:
    steadyTimer::time_point m_startTime;
    steadyTimer::time_point m_lastTime;
    steadyTimer::time_point m_nextTime;

    deltaTimeType m_totalTime;
    deltaTimeType m_deltaTime;

    unsigned int m_fpsLimit;
    std::chrono::nanoseconds m_frameTime;
public:
    cGameTimer();
    ~cGameTimer();
public:
    //타이머를 현재 시간을 기준으로 다시 시작하는 함수
    void Reset();
    //DeltaTime이 지날때까지 대기하는 함수
    deltaTimeType Update();

    deltaTimeType DeltaTime() { return m_deltaTime; }
    deltaTimeType ElapsedTime() { return m_totalTime; }

    int GetFPSLimit() { return m_fpsLimit; }
    //0프레임을 입력할시 프레임 제한 해제
    void SetFPSLimit(unsigned int _fps);
};

#define g_Timer cGameTimer::GetInst()

//singleton
//inline auto& g_Timer = cGameTimer::GetInst();