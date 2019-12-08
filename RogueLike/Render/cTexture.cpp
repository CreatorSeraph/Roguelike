#include "pch.h"
#include "cTexture.h"
#include <filesystem>

bool cTexture::Load()
{
    //이미 텍스쳐가 있는거 아니였어...?
    if (!m_texture)
        return false;

    //if (D3DXGetImageInfoFromFile(m_filePath.c_str(), &m_info) != S_OK)
    //{
    //    wclog << "cTexture::Load - Can not Get Image Info " << m_filePath << endl;
    //}
    if (D3DXCreateTextureFromFileEx(g_Device.GetDevice(), m_filePath.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, m_info.MipLevels, 0, 
          D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &m_info, nullptr, &m_texture) != S_OK)
    {
        wclog << "cTexture::Load - Fail To Create Texture " << m_filePath << endl;
        return false;
    }

    return true;
}

void cTexture::UnLoad()
{

}

cTexture::cTexture(const wstring_view& path)
    : m_texture(nullptr), m_filePath(path)
{
    Load();
}

cTexture::~cTexture()
{
    delete m_texture;
}