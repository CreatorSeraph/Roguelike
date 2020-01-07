#pragma once
#include <list>
#include <functional>
#include <thread>
#include <mutex>
#include <variant>

class cComponent;
class cComponentThread
{
protected:
    using componentIter = std::list<cComponent*>::iterator;
    componentIter m_startIter;
    cComponentThread* m_nextThread;
    int m_count;
protected:
    componentIter m_now;
    using componentFunc = void(cComponent::*)(void);
    componentFunc m_func;
protected:
    bool m_willDestroy;
    mutable std::mutex m_mutex;
    std::condition_variable& m_cv;
    std::thread m_thread;
protected:
    void InitFunc();
public:
    cComponentThread(componentIter _startIter, cComponentThread* _nextThread, std::condition_variable& _cv);
    ~cComponentThread();

    bool LaunchFunction(componentFunc _func);
public:
    componentIter GetStartIter() const { return m_startIter; }
    void SetStartIter(componentIter _iter) { m_startIter = _iter; }
    bool IsWait() const { return m_nextThread->GetStartIter() == m_now; }
    int GetCount() const { return m_count; }
    void AddComponent(int _count) { m_count += _count; }
    void DeleteComponent() { --m_count; }
};

