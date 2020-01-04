#include "pch.h"
#include "cThreadPool.h"

cThreadPool::cThreadPool(int threadSize)
    : m_isPaused(false), m_isDestroy(false)
{
    m_threads.reserve(threadSize);//미리 vector의 공간을 잡아두는 역할을 한다. 재할당이 일어나지 않게 방지할수 있다.
    for (int i = 0; i < threadSize; ++i)
    {
        m_threads.emplace_back([this]() {
            while (true)
            {
                std::unique_lock lock(m_mutex);
                //lock_gurad는 무조건 소멸자에서만 unlock을 호출 할 수 있지만 lock_guard는 unlock을 호출 할 수 있다.

                //작업이 없는 경우는 스레드 대기상태에 들어가있는다.(cpu점유를 놓아주기 위함)
                m_convar.wait(lock, [this]()
                {
                    return !m_funcQueue.empty() || m_isDestroy;
                });

                if (m_funcQueue.empty() && m_isDestroy)
                {
                    return;//할일도 없고 이제 Pool이 삭제 될때가 되었으면 thread도 일을 그만두자..
                }

                auto targetFunc = m_funcQueue.top();
                m_funcQueue.pop();//원하는 함수를 찾아서 빼고 나면

                lock.unlock();//이제 문닫고 나와서 일해야지

                targetFunc();
            }
        });
    }
}

cThreadPool::~cThreadPool()
{
    m_isDestroy = true;
    m_convar.notify_all();

    for (auto& iter : m_threads)
        iter.join();
}

void cThreadPool::Reset()
{
    std::lock_guard lock(m_mutex);
    //큐를 비어있는 상태로 초기화
    m_funcQueue = std::priority_queue<sPriorityFunction>();
}
