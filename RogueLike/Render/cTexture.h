#pragma once

class cTexture
{
protected:
    void Load();
    void UnLoad();
protected:
    D3DXIMAGE_INFO m_info;
    LPDIRECT3DTEXTURE9 m_texture;
    wstring m_filePath;
public:
    cTexture(const wstring_view& path);
    ~cTexture();
};

