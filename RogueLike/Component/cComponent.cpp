#include "pch.h"
#include "cComponent.h"

cComponent::cComponent()
    : m_obj(nullptr), m_isWillDelete(false), m_isActive(true)
{
}

cComponent::~cComponent()
{
}

inline void cComponent::SetObject(cObject* _obj)
{
    //m_obj�� *const�̱� ������ const_cast�� ���ؼ� ��������߸� �ϰ�, �ٸ������� ���� �� �Լ��� ȣ���ؼ� �ȵȴ�.
    cObject** temp = const_cast<cObject**>(&m_obj);
    *temp = _obj;
}