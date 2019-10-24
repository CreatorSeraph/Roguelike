#include "pch.h"
#include "cTimer.h"

cTimer::cTimer()
    : m_lastTime(steadyTimer::now())
{
}

cTimer::~cTimer()
{
}

deltaTimeType cTimer::Update()
{
    using namespace std::chrono;
    //����ð��� �޾ƿ�
    auto nowTime = steadyTimer::now();
    //�����ð����� ���̸� ����
    auto result = nowTime - m_lastTime;
    //����ð��� ���
    m_lastTime = nowTime;

    //��ȯ
    return duration<deltaTimeType>(result).count();
}

deltaTimeType cTimer::ElapsedTime()
{
    using namespace std::chrono;
    return duration<deltaTimeType>(steadyTimer::now() - m_lastTime).count();
}
