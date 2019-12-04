#pragma once
#include "Component/cComponent.h"

class cTexture;

class c2DRenderer : public cComponent
{
protected:
    struct sVertexType
    {
        D3DXVECTOR3 m_pos;
        D3DXVECTOR2 m_uv;

        inline static constexpr DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
    };
protected:
    cTexture* m_texturePtr;
public:
};

