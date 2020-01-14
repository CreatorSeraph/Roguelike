#include "pch.h"
#include "cObject.h"
#include "Component/cComponentManager.h"

cObject::cObject(wstring _name)
    : m_name(std::move(_name))
    , m_willDestroy(false)
    , m_dontDestroy(false)
{
}

cObject::~cObject()
{
    for (auto iter : m_components)
    {
        iter.second->Delete();
    }
}
