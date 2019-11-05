#include "pch.h"
#include "cDevice.h"

#include <filesystem>
#include <fstream>

cDevice& cDevice::GetInst()
{
    static cDevice instance;
    return instance;
}

D3DPRESENT_PARAMETERS cDevice::MakeDefaultD3Dpp(HWND _hWnd)
{
    D3DPRESENT_PARAMETERS d3dpp;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    //윈도우 사이즈에 맞춰서 생성
    d3dpp.BackBufferWidth = 0;
    d3dpp.BackBufferHeight = 0;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.hDeviceWindow = _hWnd;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    //수직동기화 on
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    //창모드 생성
    d3dpp.Windowed = true;

    return d3dpp;
}

bool cDevice::SaveNowD3Dpp()
{
    if (!std::filesystem::create_directory(L"./System")) return false;

    std::wofstream d3dppData(L"./System/d3dpp.save", std::ios::binary);
    if (!d3dppData.is_open()) return false;

    d3dppData << m_nowD3Dpp.Windowed << m_nowD3Dpp.BackBufferWidth << m_nowD3Dpp.BackBufferHeight << m_nowD3Dpp.PresentationInterval;
    return true;
}

bool cDevice::LoadNowD3Dpp()
{
    std::wifstream d3dppData(L"./System/d3dpp.save", std::ios::binary);
    if (!d3dppData.is_open()) return false;

    d3dppData >> m_nowD3Dpp.Windowed >> m_nowD3Dpp.BackBufferWidth >> m_nowD3Dpp.BackBufferHeight >> m_nowD3Dpp.PresentationInterval;
    return true;
}

cDevice::cDevice()
    :m_isInitialized(false), m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)), m_nowD3Dpp(), m_pDevice(nullptr)
{
    if(!m_d3d9)
        throw std::exception("다이렉트 x9을 지원하지 않습니다.");
}

cDevice::~cDevice()
{
    if (m_isInitialized)
        m_pDevice->Release();
    m_d3d9->Release();

    SaveNowD3Dpp();
}

HRESULT cDevice::Init(HWND _hWnd)
{
    m_nowD3Dpp = MakeDefaultD3Dpp(_hWnd);
    bool isLoadD3Dpp = LoadNowD3Dpp();

    D3DCAPS9 caps;
    m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

    DWORD vertexProcessing = 0;
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//대체 무슨 그래픽카드를 쓰시는 겁니까...

    auto CreateDevice = [&]() {
        return m_d3d9->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            _hWnd,
            vertexProcessing | D3DCREATE_MULTITHREADED,
            &m_nowD3Dpp,
            &m_pDevice);
    };

    HRESULT result = CreateDevice();

    if (result == D3D_OK)
    {
        m_isInitialized = true;
    }
    else if (result == D3DERR_INVALIDCALL && isLoadD3Dpp)
    {
        //함수 인자가 잘못되었다.
        //최근에 불러온 설정이 있었다면, 기본설정으로 초기화 해본다.
        m_nowD3Dpp = MakeDefaultD3Dpp(_hWnd);
        result = CreateDevice();
    }
    
    return result;
}

std::vector<D3DDISPLAYMODE> cDevice::GetDeviceSize()
{
    std::vector<D3DDISPLAYMODE> vecDisplayMode;

    if (m_d3d9)
    {
        size_t size = m_d3d9->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
        vecDisplayMode.reserve(size);
        for (size_t i = 0; i < size; i++)
        {
            D3DDISPLAYMODE mode;
            m_d3d9->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &mode);

            vecDisplayMode.push_back(mode);
        }
    }

    return vecDisplayMode;
}