#pragma once
#include <list>
#include <functional>
#include <thread>
#include <mutex>

class cComponent;
class cComponentThread
{
protected:
    using componentIter = std::list<cComponent*>::iterator;
    componentIter m_startIter;
    const componentIter& m_endIter;
    size_t m_count;
protected:
    componentIter m_now;
    using componentFunc = void(*)(cComponent*);
    componentFunc m_func;
protected:
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_thread;
    bool m_willDestroy;
protected:
    void InitFunc();
public:
    cComponentThread(componentIter _startIter, const componentIter& _endIter);
    ~cComponentThread();

    bool LaunchFunction(componentFunc _func);
public:
    const componentIter& GetStartIter() { return m_startIter; }
    bool IsWait() { return m_endIter == m_now; }
};

