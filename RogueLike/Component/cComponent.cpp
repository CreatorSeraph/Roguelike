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
    //m_obj가 *const이기 때문에 const_cast를 통해서 변경해줘야만 하고, 다른곳에서 절대 저 함수를 호출해선 안된다.
    cObject** temp = const_cast<cObject**>(&m_obj);
    *temp = _obj;
}