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
    int m_count;
protected:
    componentIter m_now;
    using componentFunc = void(cComponent::*)(void);
    componentFunc m_func;
protected:
    mutable std::mutex m_mutex;
    std::thread m_thread;
    bool m_willDestroy;
protected:
    void InitFunc(std::condition_variable& _cv);
public:
    cComponentThread(componentIter _startIter, const componentIter& _endIter, std::condition_variable& _cv);
    ~cComponentThread();

    bool LaunchFunction(componentFunc _func);
public:
    const componentIter& GetStartIter() const { return m_startIter; }
    bool IsWait() const { return m_endIter == m_now; }
    int GetCount() const { return m_count; }
    void AddComponent() { ++m_count; }
    void DeleteComponent() { --m_count; }
};

