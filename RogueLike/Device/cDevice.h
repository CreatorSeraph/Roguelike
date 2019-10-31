#pragma once

#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif

#pragma warning(push)
#pragma warning(disable: 26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)

#include <vector>

class cDevice
{
protected:
    D3DPRESENT_PARAMETERS MakeD3Dpp(HWND _hWnd);
    LPDIRECT3DDEVICE9 MakeDevice(HWND _hWnd);
protected:
    LPDIRECT3D9 m_d3d9;
    D3DPRESENT_PARAMETERS m_nowD3Dpp;
    LPDIRECT3DDEVICE9 m_pDevice;
public:
    cDevice(HWND _hWnd);
    ~cDevice();

    std::vector<POINT> GetDeviceSize();
};

