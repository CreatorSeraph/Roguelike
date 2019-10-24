#include "pch.h"
#include "cTimer.h"

cTimer::cTimer()
    :m_lastTime(steadyTimer::now())
{
}

cTimer::~cTimer()
{
}

deltaTimeType cTimer::Update()
{
    using namespace std::chrono;
    auto nowTime = steadyTimer::now();  //현재시간을 받아옴
    auto result = nowTime - m_lastTime; //이전시간과의 차이를 저장
    m_lastTime = nowTime;               //현재시간을 기록

    //반환
    return duration<deltaTimeType>(result).count();
}

deltaTimeType cTimer::ElapsedTime()
{
    using namespace std::chrono;
    return duration<deltaTimeType>(steadyTimer::now() - m_lastTime).count();
}
