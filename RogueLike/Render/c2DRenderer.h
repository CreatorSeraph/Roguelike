#pragma once
#include "cRenderer.h"

class cTexture;

class c2DRenderer : public cRenderer
{
protected:
    class c2DRendererIndexedBuffer
    {
        LPDIRECT3DVERTEXBUFFER9 m_vb;
        LPDIRECT3DINDEXBUFFER9 m_ib;
        cDevice::functionListIter m_releaseFuncIter;

        void Release();

        c2DRendererIndexedBuffer();
        ~c2DRendererIndexedBuffer();
    };

    MAKE_SINGLETONFUNC(c2DRendererIndexedBuffer, GetVIBuffer);
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

