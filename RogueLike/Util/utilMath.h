#pragma once

class cVector2 : public D3DXVECTOR2
{
    cVector2() = default;
    cVector2(const cVector2& other)
        : D3DXVECTOR2(other)
    {
    }
    cVector2(float x, float y)
        : D3DXVECTOR2(x, y)
    {
    }

    float Length();
    float LengthSq();
};

class cVector3 : public D3DXVECTOR3
{

};