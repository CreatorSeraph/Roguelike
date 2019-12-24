#pragma once
#include <list>
#include <functional>
#include <thread>
#include <mutex>

class cComponent;
class cComponentThread
{
protected:
    std::list<cComponent*>::iterator m_startIter;
    std::list<cComponent*>::iterator m_endIter;
    size_t m_count;

    std::list<cComponent*>::iterator m_now;
    std::function<void(cComponent*)> m_function;
protected:
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
public:
    cComponentThread();
    ~cComponentThread();

    void LaunchFunction(std::function<void(cComponent*)> _func);
};

