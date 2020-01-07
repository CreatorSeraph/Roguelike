#include "pch.h"
#include "cComponentThread.h"

void cComponentThread::InitFunc()
{
    while (true)
    {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this]() {
            return (m_nextThread->GetStartIter() == m_now && m_func)
                || m_willDestroy;
        });
        lock.unlock();
        if (m_willDestroy)
            break;
    
        for (m_now = m_startIter; m_now != m_nextThread->GetStartIter(); ++m_now)
        {
            ((*m_now)->*m_func)();
        }
        m_func = nullptr;
    }
}

cComponentThread::cComponentThread(componentIter _startIter, cComponentThread* _nextThread, std::condition_variable& _cv)
    : m_startIter(_startIter)
    , m_nextThread(_nextThread)
    , m_count(std::distance(_startIter, _nextThread->GetStartIter()))
    , m_now(_nextThread->GetStartIter())
    , m_func(nullptr)
    , m_willDestroy(false)
    , m_cv(_cv)
    , m_thread(&cComponentThread::InitFunc, this)
{
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
