#pragma once
#include <list>

class cObject;
class cObjectManager
{
public:
    MAKE_SINGLETONFUNC(cObjectManager, GetInst)
protected:
    std::list<cObject*> m_objList;
    std::list<cObject*> m_reservedObjList;
public:
    cObjectManager();
    ~cObjectManager();
public:
    void AddObject(cObject* _obj);
    void Update();
    void OnSceneChange();
public:
    void DontDestroyOnSceneChange(cObject* _obj);
};

