#include "pch.h"
#include "cComponentManager.h"
#include "cComponent.h"
#include "Thread/cComponentThread.h"
#include <algorithm>

cComponentManager::cComponentManager(size_t _threadCount)
    : m_endIter(m_components.end())
{
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
        auto minCountThread = std::min_element(m_componentThreads.rbegin(), m_componentThreads.rend(), []() {});

        m_components.splice((*minCountThread)->GetStartIter(), m_reservedComponents, m_reservedComponents.begin());
        (*minCountThread)->AddComponent();
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
}
