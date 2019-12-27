#include "pch.h"
#include "cComponentThread.h"

void cComponentThread::InitFunc(std::condition_variable& _cv)
{
    while (true)
    {
        std::unique_lock lock(m_mutex);
        _cv.wait(lock, [this]() {
            return (m_endIter == m_now && m_func)
                || m_willDestroy;
        });
        lock.unlock();

        for (m_now = m_startIter; m_now != m_endIter; ++m_now)
        {
            ((*m_now)->*m_func)();
        }
        m_func = nullptr;
    }
}

cComponentThread::cComponentThread(componentIter _startIter, const componentIter& _endIter, std::condition_variable& _cv)
    : m_startIter(_startIter), m_endIter(_endIter), m_count(std::distance(_startIter, _endIter))
    , m_now(_endIter), m_willDestroy(false), m_func(nullptr)
    , m_thread([this, &_cv]() {InitFunc(_cv); })
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
