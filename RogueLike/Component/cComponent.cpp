#include "pch.h"
#include "cComponent.h"

cComponent::cComponent()
    : m_obj(nullptr), m_isWillDelete(false), m_isActive(true)
{
}

cComponent::~cComponent()
{
}