#include "pch.h"
#include "cGameTimer.h"

#include <thread>

cGameTimer& cGameTimer::GetInst()
{
    //�̱���
    static cGameTimer instance;
    return instance;
}

cGameTimer::cGameTimer()
    :m_startTime(m_lastTime), m_nextTime(m_lastTime),
    m_totalTime(0), m_deltaTime(0),
    m_fpsLimit(60), m_frameTime(1000000000ns / 60)
    //1s = 1000000000ns
{
}

cGameTimer::~cGameTimer()
{
}

void cGameTimer::Reset()
{
    m_startTime = m_lastTime = steadyTimer::now();
    m_totalTime = 0;
}

deltaTimeType cGameTimer::Update()
{
    using namespace std::chrono;
    auto nowTime = steadyTimer::now();
    while (m_nextTime > nowTime)
    {
        std::this_thread::yield();
        nowTime = steadyTimer::now();
    }
    
    //���� �������� ���
    m_lastTime = nowTime;
    //������ deltaTime���� ���� ���������� �ð�
    auto checkedLastTime = (m_startTime + duration_cast<nanoseconds>(duration<deltaTimeType>(m_totalTime)));
    //�������������κ��� �����ð� ���(�ε��Ҽ��� ������ �����ؼ�)
    m_deltaTime = duration<deltaTimeType>(m_lastTime - checkedLastTime).count();
    //deltaTime�� ����
    m_totalTime += m_deltaTime;
    //���������ӿ� ������Ʈ �Ǿ���� �ð��� ���
    m_nextTime = m_lastTime + m_frameTime;
    return m_deltaTime;
}

void cGameTimer::SetFPSLimit(unsigned int _fps)
{
    using namespace std::chrono;
    m_fpsLimit = _fps;
    m_frameTime = (_fps == 0) ? 0ns : (1000000000ns / _fps);
}
