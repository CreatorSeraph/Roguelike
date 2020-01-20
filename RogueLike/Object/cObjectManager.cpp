#include "pch.h"
#include "cObjectManager.h"
#include "cObject.h"

cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{
}

void cObjectManager::AddObject(cObject* _obj)
{
    m_reservedObjList.push_back(_obj);
}

void cObjectManager::Update()
{
    m_objList.splice(m_objList.end(), m_reservedObjList);
}

void cObjectManager::OnSceneChange()
{
    for (auto iter = m_objList.begin(); iter != m_objList.end(); )
    {
        if ((*iter)->m_dontDestroy)
            ++iter;
        else
            iter = m_objList.erase(iter);
    }
}

void cObjectManager::DontDestroyOnSceneChange(cObject* _obj)
{
    _obj->m_dontDestroy = true;
}
