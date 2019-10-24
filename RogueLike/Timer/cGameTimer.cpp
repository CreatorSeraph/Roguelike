#include "pch.h"
#include "cGameTimer.h"

#include <thread>

cGameTimer& cGameTimer::GetInst()
{
    //싱글톤
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
    
    //현재 프레임을 기록
    m_lastTime = nowTime;
    //누적된 deltaTime으로 계산된 이전프레임 시간
    auto checkedLastTime = (m_startTime + duration_cast<nanoseconds>(duration<deltaTimeType>(m_totalTime)));
    //이전프레임으로부터 지난시간 계산(부동소수점 오차를 보정해서)
    m_deltaTime = duration<deltaTimeType>(m_lastTime - checkedLastTime).count();
    //deltaTime을 누적
    m_totalTime += m_deltaTime;
    //다음프레임에 업데이트 되어야할 시간을 기록
    m_nextTime = m_lastTime + m_frameTime;
    return m_deltaTime;
}

void cGameTimer::SetFPSLimit(unsigned int _fps)
{
    using namespace std::chrono;
    m_fpsLimit = _fps;
    m_frameTime = (_fps == 0) ? 0ns : (1000000000ns / _fps);
}
