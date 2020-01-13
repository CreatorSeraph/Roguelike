#pragma once
#include <unordered_set>

class cObject;
class cObjectManager
{
public:
    MAKE_SINGLETONFUNC(cObjectManager, GetInst)
protected:
    std::unordered_multiset<cObject*> m_objList;
    //씬 전환시에도 삭제되지 않을 오브젝트
    std::unordered_multiset<cObject*> m_dontDestroyObjList;
public:
    cObjectManager();
    ~cObjectManager();

public:
    void AddObject(cObject* _obj);
};

