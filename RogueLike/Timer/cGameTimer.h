#pragma once
#include "cTimer.h"

class cGameTimer : public cTimer
{
public:
    static cGameTimer& GetInst();
protected:
    steadyTimer::time_point m_startTime;
    steadyTimer::time_point m_nextTime;

    deltaTimeType m_totalTime;
    deltaTimeType m_deltaTime;

    unsigned int m_fpsLimit;
    std::chrono::nanoseconds m_frameTime;
public:
    cGameTimer();
    ~cGameTimer();
public:
    //Ÿ�̸Ӹ� ���� �ð��� �������� �ٽ� �����ϴ� �Լ�
    void Reset();
    //DeltaTime�� ���������� ����ϴ� �Լ�
    deltaTimeType Update();
    deltaTimeType DeltaTime() { return m_deltaTime; }

    int GetFPSLimit() { return m_fpsLimit; }
    //0�������� �Է��ҽ� ������ ���� ����
    void SetFPSLimit(unsigned int _fps);
};

//define��� ���������� ����� �ߴ�.
//#define TIMER cGameTimer::GetInst()

//singleton
inline auto& g_Timer = cGameTimer::GetInst();