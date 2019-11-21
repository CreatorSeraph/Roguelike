#include "pch.h"
#include "c2DTransform.h"

void c2DTransform::RenewalMatrix()
{
    auto sinRot = sin(m_rot);
    auto cosRot = cos(m_rot);
    auto pivotX = (m_pivotPos.x - 0.5f) * m_size.x;
    auto pivotY = (m_pivotPos.y - 0.5f) * m_size.y;

    auto _41 = -cosRot * pivotX * m_scale.x + sinRot * pivotY * m_scale.y + m_pos.x;
    auto _42 = sinRot * pivotX * m_scale.x + cosRot * pivotY * m_scale.y + m_pos.y;

    m_matrix._11 = cosRot * m_scale.x;  m_matrix._12 = -sinRot * m_scale.x; m_matrix._13 = 0;       m_matrix._14 = 0;
    m_matrix._21 = sinRot * m_scale.y;  m_matrix._22 = cosRot * m_scale.x;  m_matrix._23 = 0;       m_matrix._24 = 0;
    m_matrix._31 = 0;                   m_matrix._32 = 0;                   m_matrix._33 = 1;       m_matrix._34 = 0;
    m_matrix._41 = _41;                 m_matrix._42 = _42;                 m_matrix._43 = m_pos.z; m_matrix._44 = 1;
}