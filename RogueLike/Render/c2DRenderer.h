#pragma once
#include "cRenderer.h"

class cTexture;

class c2DRenderer : public cRenderer
{
protected:
    struct s2DRendererIndexedBuffer
    {
        LPDIRECT3DVERTEXBUFFER9 m_vb;
        LPDIRECT3DINDEXBUFFER9 m_ib;
        cDevice::functionListIter m_releaseFuncIter;

        void Release();

        s2DRendererIndexedBuffer();
        ~s2DRendererIndexedBuffer();
    };

    MAKE_SINGLETONFUNC(s2DRendererIndexedBuffer, GetVIBuffer);
protected:
    static LPDIRECT3DVERTEXBUFFER9 staticVB;
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

