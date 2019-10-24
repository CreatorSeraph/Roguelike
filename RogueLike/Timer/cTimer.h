#pragma once
#include <chrono>
#include "timerType.h"

class cTimer
{
protected:
    steadyTimer::time_point   m_lastTime;     //�������� Ÿ�̸Ӹ� Ȯ���� ����
public:
    cTimer();
    ~cTimer();
public:
    deltaTimeType Update();         //lastTime�� �����ϰ� lastTime���� �����ð�
    deltaTimeType ElapsedTime();    //Ÿ�̸Ӱ� �����ϰ� ��������� ��� �ð�
    steadyTimer::time_point LastTime() { return m_lastTime; }
};

