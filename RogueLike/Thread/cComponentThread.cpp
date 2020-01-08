#include "pch.h"
#include "cComponentThread.h"

void cComponentThread::InitFunc(std::condition_variable& _cv)
{
    while (true)
    {
        std::unique_lock lock(m_mutex);
        _cv.wait(lock, [this]() {
            return (GetEndIter() == m_now && m_func)
                || m_willDestroy;
        });
        lock.unlock();
        if (m_willDestroy)
            break;
    
        for (m_now = m_startIter; m_now != GetEndIter(); ++m_now)
        {
            m_func(*m_now);
            //((*m_now)->*m_func)();
        }
        m_func = nullptr;
    }
}

cComponentThread::cComponentThread(componentIter _startIter, cComponentThread* _nextThread, componentIter _endIter, std::condition_variable& _cv)
    : m_startIter(_startIter)
    , m_nextThread(_nextThread)
    , m_endIter(_endIter)
    , m_count(std::distance(_startIter, (_nextThread ? _nextThread->GetStartIter() : _endIter)))
    , m_now((_nextThread ? _nextThread->GetStartIter() : _endIter))
    , m_func()
    , m_willDestroy(false)
    , m_thread([this, &_cv]() { InitFunc(_cv); })
{
    //m_thread = std::thread();
}

cComponentThread::~cComponentThread()
{
    m_willDestroy = true;
    m_thread.join();
}

bool cComponentThread::LaunchFunction(componentFunc _func)
{
    if (m_func)
        return false;

    std::lock_guard lock(m_mutex);
    m_func = _func;
    return true;
}
