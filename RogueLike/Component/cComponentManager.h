#pragma once
#include <list>

class cComponent;
class cComponentManager
{
public:
    MAKE_SINGLETONFUNC(cComponentManager, GetInst)
protected:
    std::list<cComponent*> m_components;
public:
    void AddComponent(cComponent* component);
    void Update();
};

#define COMPONENTMANAGER cDevice::GetInst()