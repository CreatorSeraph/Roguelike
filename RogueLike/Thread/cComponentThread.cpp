#include "pch.h"
#include "cComponentThread.h"

void cComponentThread::InitFunc(std::condition_variable& _cv, std::mutex& _m)
{
    while (true)
    {
        std::unique_lock lock(_m);

        _cv.wait(lock, [&]() {
            return (m_endIter == m_now && m_func) || m_willDestroy;
        });
        lock.unlock();
        if (m_willDestroy)
            break;
    
        for (m_now = m_startIter; m_now != m_endIter; ++m_now)
        {
            ((*m_now)->*m_func)();
        }
        m_func = nullptr;
    }
}

cComponentThread::cComponentThread(const componentIter& _endIter, bool& _destroyCondition)
    : m_endIter(_endIter)
    , m_willDestroy(_destroyCondition)
{
}

cComponentThread::cComponentThread(componentIter _startIter, const componentIter& _endIter, std::condition_variable& _cv, std::mutex& _m, bool& _destroyCondition)
    : m_startIter(_startIter)
    , m_endIter(_endIter)
    , m_count(std::distance(_startIter, _endIter))
    , m_now(_endIter)
    , m_func(nullptr)
    , m_willDestroy(_destroyCondition)
    , m_thread([&]() {InitFunc(_cv, _m); })
{
}

cComponentThread::~cComponentThread()
{
    m_thread.join();
}

bool cComponentThread::LaunchFunction(componentFunc _func)
{
    if (m_func)
        return false;

    m_func = _func;
    return true;
}
