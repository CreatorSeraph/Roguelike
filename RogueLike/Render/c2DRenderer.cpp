#include "pch.h"
#include "c2DRenderer.h"

c2DRenderer::s2DRendererIndexedBuffer::s2DRendererIndexedBuffer()
{
    g_Device.GetDevice()->CreateVertexBuffer(
        sizeof(sVertexType) * 4,
        D3DUSAGE_WRITEONLY,
        sVertexType::FVF,
        D3DPOOL_DEFAULT,
        &m_vb,
        NULL);

    sVertexType* pVertices = nullptr;

    m_vb->Lock(0, 0, (void**)&pVertices, 0);

    pVertices[0].m_pos = D3DXVECTOR3(-1.f, -1.f, 0.f);
    pVertices[1].m_pos = D3DXVECTOR3(-1.f, 1.f, 0.f);
    pVertices[2].m_pos = D3DXVECTOR3(1.f, 1.f, 0.f);
    pVertices[3].m_pos = D3DXVECTOR3(1.f, -1.f, 0.f);

    pVertices[0].m_uv = D3DXVECTOR2(1, 0);
    pVertices[1].m_uv = D3DXVECTOR2(0, 0);
    pVertices[2].m_uv = D3DXVECTOR2(0, 1);
    pVertices[3].m_uv = D3DXVECTOR2(1, 1);

    m_vb->Unlock();

    g_Device.GetDevice()->CreateIndexBuffer(
        sizeof(WORD) * 6,
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_DEFAULT,
        &m_ib,
        NULL);

    WORD idx[] = { 0, 1, 2, 0, 2, 3 };

    void* pIndices = NULL;

    m_ib->Lock(0, 0, &pIndices, 0);
    memcpy(pIndices, idx, sizeof(WORD) * 6);
    m_ib->Unlock();

    m_releaseFuncIter = g_Device.AddReleaseDeviceFunc([this]() {Release(); });
}

void c2DRenderer::s2DRendererIndexedBuffer::Release()
{
    if (m_ib)
    {
        m_ib->Release();
        m_ib = nullptr;
    }
    if (m_vb)
    {
        m_vb->Release();
        m_vb = nullptr;
    }
}

c2DRenderer::s2DRendererIndexedBuffer::~s2DRendererIndexedBuffer()
{
    g_Device.DeleteReleaseDeviceFunc(m_releaseFuncIter);
}
