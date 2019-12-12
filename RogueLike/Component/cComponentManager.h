#pragma once
#include <list>

class cComponent;
class cComponentManager
{
public:
    MAKE_SINGLETONFUNC(cComponentManager, GetInst)
protected:
    std::list<cComponent* const> m_components;
public:
    void AddComponent(cComponent* const component);
    void Update();
};

#define COMPONENTMANAGER cDevice::GetInst()