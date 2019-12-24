#pragma once
#include <list>

class cComponent;
class cComponentManager
{
public:
    MAKE_SINGLETONFUNC(cComponentManager, GetInst)
protected:
    std::list<cComponent*> m_components;
    std::list<cComponent*>::iterator m_endIter;

    std::list<cComponent*> m_reservedComponents;//������ �߰���
public:
    void AddComponent(cComponent* component);
    void Update();
    void LateUpdate();
    void AfterUpdate();
};

#define COMPONENTMANAGER cDevice::GetInst()