#pragma once
#include <map>
#include <type_traits>
#include "Component/cComponent.h"

class cObjectManager;
class cObject
{
    friend cObjectManager;
protected:
    std::map<string, cComponent*> m_components;
    wstring m_name;
    bool m_willDestroy;
    //씬 전환시에도 오브젝트가 삭제되지 않게 한다.
    bool m_dontDestroy;
public:
    cObject(wstring _name = L""s);
    virtual ~cObject();
public:
    template<class ComponentType, class = std::enable_if_t<std::is_base_of_v<cComponent, ComponentType>>, class ...Args>
    ComponentType* AddComponent(Args&&... args)
    {
        string key = typeid(ComponentType).name();

        if (m_components.find(key) == m_components.end())
        {
            cComponent* newComponent = new ComponentType(std::forward(args)...);
            newComponent->SetObject(this);
            newComponent->OnCreate();

            m_components.insert(std::make_pair(key, newComponent));

            return newComponent;
        }

        return nullptr;
    }

    template<class ComponentType, class = std::enable_if_t<std::is_base_of_v<cComponent, ComponentType>>>
    ComponentType* GetComponent()
    {
        string key = typeid(ComponentType).name();

        if (auto find = m_components.find(key); find == m_components.end())
            return find->second;

        return nullptr;
    }
};