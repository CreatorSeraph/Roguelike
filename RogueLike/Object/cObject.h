#pragma once
#include <map>
#include <type_traits>
#include "Component/cComponent.h"

class cObject
{
protected:
    std::map<string, cComponent*> m_components;
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