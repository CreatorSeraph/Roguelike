#pragma once
class c2DTransform
{
protected:
    //x, y  : pos
    //z     : z order
    D3DXVECTOR3 m_pos;
    D3DXVECTOR2 m_pivotPos;
    D3DXVECTOR2 m_scale;
    D3DXVECTOR2 m_size;
    float m_rot;

    D3DXMATRIX m_matrix;
protected:
    void RenewalMatrix();
public:
    c2DTransform();
    ~c2DTransform();

    const D3DXVECTOR3& GetPos() { return m_pos; }
    const D3DXVECTOR2& GetPivotPos() { return m_pivotPos; }
    const D3DXVECTOR2& GetScale() { return m_scale; }
    const D3DXVECTOR2& GetSize() { return m_size; }
    float GetRot() { return m_rot; }

    const D3DXMATRIX& GetMatrix() { return m_matrix; }
};

