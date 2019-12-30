#include "pch.h"
#include "cObject.h"
#include "Component/cComponentManager.h"

cObject::cObject(wstring _name)
    : m_name(std::move(_name))
{
}

cObject::~cObject()
{
    for (auto iter : m_components)
    {
        iter.second->Delete();
    }
}
