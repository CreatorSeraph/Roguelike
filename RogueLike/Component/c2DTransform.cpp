#include "pch.h"
#include "c2DTransform.h"

void c2DTransform::RenewalMatrix()
{
    m_worldMatrix = GetLocalMatrix();
    if (m_parent)
        m_worldMatrix *= m_parent->GetWorldMatrix();

    m_isNeedRenewalWorldMatrix = false;
}

c2DTransform::c2DTransform()
    : m_isNeedRenewalWorldMatrix(false)
    , m_parent(nullptr), m_rot(0.f)
{
}

c2DTransform::~c2DTransform()
{
}

D3DXMATRIX c2DTransform::GetLocalMatrix()
{
    D3DXMATRIX result;

    auto sinRot = sin(m_rot);
    auto cosRot = cos(m_rot);
    auto pivotX = (m_pivotPos.x - 0.5f) * m_size.x;
    auto pivotY = (m_pivotPos.y - 0.5f) * m_size.y;

    auto _41 = -cosRot * pivotX * m_scale.x + sinRot * pivotY * m_scale.y + m_pos.x;
    auto _42 = sinRot * pivotX * m_scale.x + cosRot * pivotY * m_scale.y + m_pos.y;

    result._11 = cosRot * m_scale.x;  result._12 = -sinRot * m_scale.x; result._13 = 0;       result._14 = 0;
    result._21 = sinRot * m_scale.y;  result._22 = cosRot * m_scale.x;  result._23 = 0;       result._24 = 0;
    result._31 = 0;                   result._32 = 0;                   result._33 = 1;       result._34 = 0;
    result._41 = _41;                 result._42 = _42;                 result._43 = m_pos.z; result._44 = 1;

    return result;
}

const D3DXMATRIX& c2DTransform::GetWorldMatrix()
{
    if (m_isNeedRenewalWorldMatrix)
        RenewalMatrix();

    return m_worldMatrix;
}
