#pragma once
#include "Component/cComponent.h"

class cRenderer : public cComponent
{
public:
    cRenderer();
    ~cRenderer();
public:
    virtual void Render() = 0;
};

