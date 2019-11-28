#pragma once
#include <list>
#include "Component/cComponent.h"

class c2DTransform : public cComponent
{
protected:
    bool m_isNeedRenewalWorldMatrix;
    D3DXMATRIX m_worldMatrix;
    //x, y  : pos
    //z     : z order
    D3DXVECTOR3 m_pos;
    D3DXVECTOR2 m_pivotPos;
    D3DXVECTOR2 m_scale;
    D3DXVECTOR2 m_size;
    float m_rot;

    c2DTransform* m_parent;
    //std::list<c2DTransform*> m_childs;
protected:
    void RenewalMatrix();
public:
    c2DTransform(cObject* _obj);
    ~c2DTransform();

    D3DXVECTOR3 GetPos() { return m_pos; }
    D3DXVECTOR2 GetPivotPos() { return m_pivotPos; }
    D3DXVECTOR2 GetScale() { return m_scale; }
    D3DXVECTOR2 GetSize() { return m_size; }
    float GetRot() { return m_rot; }

    D3DXMATRIX GetLocalMatrix();
    const D3DXMATRIX& GetWorldMatrix();
};
