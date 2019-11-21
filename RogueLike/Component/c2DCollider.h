#pragma once
#include "Component/cComponent.h"
#include "Component/c2DTransform.h"

class c2DCollider : public cComponent
{
    c2DTransform* m_transform;
};

