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

#include <list>
#include <vector>
#include <functional>

class cDevice
{
public:
    MAKE_SINGLETONFUNC(cDevice, GetInst)
protected:
    D3DPRESENT_PARAMETERS MakeDefaultD3Dpp(HWND _hWnd);
    bool SaveNowD3Dpp();
    bool LoadNowD3Dpp();

    inline void BeforeReleaseDevice();
    inline void OnLostDevice();
    inline void OnResetDevice();
protected:
    bool m_isInitialized;
    LPDIRECT3D9 m_d3d9;
    D3DPRESENT_PARAMETERS m_nowD3Dpp;
    LPDIRECT3DDEVICE9 m_pDevice;

    D3DCOLOR m_clearColor;

    bool m_isLost;
    bool m_needResetObj;

    using functionList = std::list<std::function<void()>>;
    functionList m_releaseDeviceFunc;
    functionList m_lostDeviceFunc;
    functionList m_resetDeviceFunc;
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
public:
    using functionListIter = functionList::iterator;
    functionListIter AddReleaseDeviceFunc(std::function<void()> _func);
    functionListIter AddLostDeviceFunc(std::function<void()> _func);
    functionListIter AddResetDeviceFunc(std::function<void()> _func);
    void DeleteReleaseDeviceFunc(functionListIter _iter);
    void DeleteLostDeviceFunc(functionListIter _iter);
    void DeleteResetDeviceFunc(functionListIter _iter);
public:
    bool IsLostDevice() { return m_isLost; }

    LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
    std::vector<D3DDISPLAYMODE> GetDeviceSize();

    operator bool() { return m_d3d9; }
};

//singleton
#define g_Device cDevice::GetInst()
//inline auto& g_Device = cDevice::GetInst();
//아래 방법이면 GetInst함수를 호출하는게 아니라 그걸로 변수를 초기화 해버린다 ;-;