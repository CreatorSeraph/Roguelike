#pragma once
#include <unordered_set>

class cObject;
class cObjectManager
{
public:
    MAKE_SINGLETONFUNC(cObjectManager, GetInst)
protected:
    std::unordered_multiset<cObject*> m_objList;
    //�� ��ȯ�ÿ��� �������� ���� ������Ʈ
public:
    cObjectManager();
    ~cObjectManager();

public:
    void AddObject(cObject* _obj);
};
