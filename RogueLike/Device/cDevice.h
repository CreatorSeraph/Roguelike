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
#include <functional>

class cDevice
{
public:
    static cDevice& GetInst();
protected:
    D3DPRESENT_PARAMETERS MakeDefaultD3Dpp(HWND _hWnd);
    bool SaveNowD3Dpp();
    bool LoadNowD3Dpp();
protected:
    bool m_isInitialized;
    LPDIRECT3D9 m_d3d9;
    D3DPRESENT_PARAMETERS m_nowD3Dpp;
    LPDIRECT3DDEVICE9 m_pDevice;

    D3DCOLOR m_clearColor;

    bool m_isLost;
    bool m_needResetObj;

    std::function<void()> m_lostDeviceFunc;
    std::function<void()> m_resetDeviceFunc;
public:
    cDevice();
    ~cDevice();
public:
    HRESULT Init(HWND _hWnd);
    HRESULT Clear();
    HRESULT BeginScene();
    HRESULT EndScene();
    HRESULT Present();
    HRESULT TryDeviceReset();
    HRESULT ChangeDevice();

    bool isLostDevice() { return m_isLost; }

    std::vector<D3DDISPLAYMODE> GetDeviceSize();
};

//singleton
inline auto& g_Device = cDevice::GetInst();