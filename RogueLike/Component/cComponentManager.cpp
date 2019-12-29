#include "pch.h"
#include "cComponentManager.h"
#include "cComponent.h"
#include "Thread/cComponentThread.h"
#include <algorithm>

cComponentManager::cComponentManager(size_t _threadCount)
    : m_endIter(m_components.end()), m_threadCount(_threadCount)
{
    if (_threadCount == 0)
        throw std::exception("_threadCount는 0일수 없습니다.");
    m_componentThreads.resize(_threadCount);
    for (int i = _threadCount - 1; i >= 0; --i)
    {
        m_componentThreads[i] = new cComponentThread(m_components.begin(),
            (i == _threadCount - 1) ?
            m_endIter :
            m_componentThreads[i + 1]->GetStartIter(),
            m_cv);
    }
}

cComponentManager::~cComponentManager()
{
    for (auto iter : m_componentThreads)
        delete iter;
}

void cComponentManager::AddComponent(cComponent* _component)
{
    m_reservedComponents.push_back(_component);
}

void cComponentManager::BeforeUpdate()
{
    while (!m_reservedComponents.empty())
    {
        //auto minCountThread = std::min_element(m_componentThreads.rbegin(), m_componentThreads.rend(),
        //    [](const cComponentThread* p1, const cComponentThread* p2) {
        //        return p1->GetCount() < p2->GetCount();
        //    });
        cComponentThread* minCountThread = m_componentThreads[0];
        int minGap = m_reservedComponents.size();
        if (m_threadCount > 1)
        {
            for (int i = 1; i < m_threadCount; ++i)
            {
                cComponentThread* iter = m_componentThreads[i];
                int gap = iter->GetCount() - minCountThread->GetCount();

                if (gap < minGap)
                {
                    if (gap < 0)
                    {
                        minCountThread = iter;
                        gap = -gap;
                    }
                    minGap = gap;
                }
            }
            //cComponentThread* minSecCountThread = m_componentThreads[1];
            //if (minCountThread->GetCount() > minSecCountThread->GetCount())
            //    std::swap(minCountThread, minSecCountThread);
            //
            //for (int i = 2; i < m_threadCount; ++i)
            //{
            //    auto* iter = m_componentThreads[i];
            //    auto count = iter->GetCount();
            //    if (count < minSecCountThread->GetCount())
            //    {
            //        minSecCountThread = iter;
            //        if (count < minCountThread->GetCount())
            //        {
            //            std::swap(minCountThread, minSecCountThread);
            //        }
            //    }
            //}
        }

        m_components.splice(minCountThread->GetStartIter(), m_reservedComponents, m_reservedComponents.begin());
        minCountThread->AddComponent();
    }
}

void cComponentManager::Update()
{
    for (auto iter : m_componentThreads)
        iter->LaunchFunction(&cComponent::OnUpdate);
    m_cv.notify_all();
}

void cComponentManager::LateUpdate()
{
    for (auto iter : m_componentThreads)
        iter->LaunchFunction(&cComponent::OnLateUpdate);
    m_cv.notify_all();
}

void cComponentManager::AfterUpdate()
{
    for (auto iter = m_components.begin(); iter != m_components.end();)
    {

    }

}
