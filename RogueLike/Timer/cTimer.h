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
    //lastTime�� �����ϰ� lastTime���� �����ð�
    deltaTimeType Update();
    //Ÿ�̸Ӱ� �����ϰ� ��������� ��� �ð�
    deltaTimeType ElapsedTime();
    steadyTimer::time_point LastTime() { return m_lastTime; }
};

