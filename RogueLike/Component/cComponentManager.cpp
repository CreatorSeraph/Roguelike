#include "pch.h"
#include "cComponentManager.h"
#include "cComponent.h"
#include "Thread/cComponentThread.h"
#include <algorithm>

cComponentManager::cComponentManager(size_t _threadCount)
    : m_threadCount(_threadCount)
{
    if (_threadCount == 0)
        throw std::exception("_threadCount는 0일수 없습니다.");
    m_componentThreads.resize(_threadCount);
    for (int i = _threadCount - 1; i >= 0; --i)
    {
        //m_componentThreads[i] = new cComponentThread();
        //m_componentThreads[i] = new cComponentThread(nullptr);
        m_componentThreads[i] = new cComponentThread(m_components.begin(),
            ((i == _threadCount - 1) ?
            nullptr :
            m_componentThreads[i + 1]),
            m_components.end(),
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
        std::vector<cComponentThread*> minCountThread;
        minCountThread.reserve(m_threadCount);
        minCountThread.push_back(m_componentThreads[0]);
        int reservedCount = m_reservedComponents.size();

        //가장 적은 개수의 컴포넌트를 가지고 있는 스레드와, 넣어도 괜찮은 최대 컴포넌트 개수를 구한다.
        int minCount = m_componentThreads[0]->GetCount();
        int minGap = reservedCount;
        if (m_threadCount > 1)
        {
            for (int i = 1; i < m_threadCount; ++i)
            {
                cComponentThread* iter = m_componentThreads[i];
                int gap = iter->GetCount() - minCount;

                if (minGap == 0 && gap == 0)
                {
                    minCountThread.push_back(iter);
                }
                else if (gap < minGap)
                {
                    if (gap < 0)
                    {
                        minCountThread.clear();
                        minCountThread.push_back(iter);
                        gap = -gap;
                        minCount = 1;
                    }
                    minGap = gap;
                }
            }
        }

        int minThreadCount = minCountThread.size();
        int totalCount = minThreadCount * minGap;
        if (totalCount > reservedCount)
            totalCount = reservedCount;

        auto divResult = std::div(totalCount, minThreadCount);

        //가장 적은 개수의 컴포넌트를 가진 스레드에 필요한 개수만큼 나눠서 컴포넌트를 넣는다.
        for (int i = 0; i < minThreadCount; ++i)
        {
            auto* iter = minCountThread[i];

            int moveCount = divResult.quot + (i < divResult.rem);
            auto startIter = std::prev(m_reservedComponents.end(), moveCount);
            m_components.splice(iter->GetStartIter(), m_reservedComponents, startIter, m_reservedComponents.end());
            iter->AddComponent(moveCount);

            iter->SetStartIter(startIter);
        }
    }
}

void cComponentManager::Update()
{
    for (auto* iter : m_componentThreads)
        iter->LaunchFunction(&cComponent::OnUpdate);
    m_cv.notify_all();
}

void cComponentManager::LateUpdate()
{
    for (auto* iter : m_componentThreads)
        iter->LaunchFunction(&cComponent::OnLateUpdate);
    m_cv.notify_all();
}

void cComponentManager::AfterUpdate()
{
    for (auto* iter : m_componentThreads)
    {
        auto GetNextIter = [this](cComponentThread* _iter) {
            auto nextThread = _iter->GetNextThread();
            if (nextThread)
                return nextThread->GetStartIter();
            return m_components.end();
        };
        auto targetEndIter = GetNextIter(iter);
        for (auto componentIter = iter->GetStartIter(); componentIter != targetEndIter; )
        {
            if ((*componentIter)->IsWillDelete())
            {
                bool isStartIter = componentIter == iter->GetStartIter();
                componentIter = m_components.erase(componentIter);
                if (isStartIter)
                    iter->SetStartIter(componentIter);
                iter->DeleteComponent();
            }
            else
            {
                ++componentIter;
            }
        }
    }
}
