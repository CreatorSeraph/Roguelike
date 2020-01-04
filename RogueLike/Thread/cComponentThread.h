#pragma once
#include <list>
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
    bool& m_willDestroy;
    std::thread m_thread;
protected:
    void InitFunc(std::condition_variable& _cv, std::mutex& _m);
public:
    cComponentThread(const componentIter& _endIter, bool& _destroyCondition);
    cComponentThread(componentIter _startIter, const componentIter& _endIter, std::condition_variable& _cv, std::mutex& _m, bool& _destroyCondition);
    ~cComponentThread();

    bool LaunchFunction(componentFunc _func);
public:
    const componentIter& GetStartIter() const { return m_startIter; }
    componentIter GetEndIter() const { return m_endIter; }
    void SetStartIter(componentIter _iter) { m_startIter = _iter; }
    bool IsWait() const { return m_endIter == m_now; }
    int GetCount() const { return m_count; }
    void AddComponent(int _count) { m_count += _count; }
    void DeleteComponent() { --m_count; }
};

