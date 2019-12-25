#include "pch.h"
#include "cComponentManager.h"
#include "cComponent.h"
#include "Thread/cComponentThread.h"

cComponentManager::cComponentManager(size_t _threadCount)
    : m_endIter(m_components.end())
{
    m_componentThreads.resize(_threadCount);
    for (int i = _threadCount - 1; i >= 0; --i)
    {
        m_componentThreads[i] = new cComponentThread(m_components.begin(),
            (i == _threadCount - 1) ?
            m_endIter :
            m_componentThreads[i + 1]->GetStartIter());
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
}

void cComponentManager::Update()
{
}

void cComponentManager::LateUpdate()
{
}

void cComponentManager::AfterUpdate()
{
}
