#include "pch.h"
#include "cDevice.h"

D3DPRESENT_PARAMETERS cDevice::MakeD3Dpp(HWND _hWnd)
{
    D3DPRESENT_PARAMETERS d3dpp;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = 1280;
    d3dpp.BackBufferHeight = 720;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.hDeviceWindow = _hWnd;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.Windowed = true;

    return d3dpp;
}

LPDIRECT3DDEVICE9 cDevice::MakeDevice(HWND _hWnd)
{
    if (!m_d3d9)
    {
        throw std::exception("다이렉트 x9을 지원하지 않습니다.");
    }

    LPDIRECT3DDEVICE9 device;
    D3DCAPS9	caps;
    m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

    DWORD vertexProcessing = 0;
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//대체 무슨 그래픽카드를 쓰시는 겁니까...

    HRESULT result = m_d3d9->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        _hWnd,
        vertexProcessing | D3DCREATE_MULTITHREADED,
        &m_nowD3Dpp,
        &device);

    return device;
}

cDevice::cDevice(HWND _hWnd)
    :m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)), m_nowD3Dpp(MakeD3Dpp(_hWnd)), m_pDevice(MakeDevice(_hWnd))
{
    D3DCAPS9	caps;
    m_d3d9->GetDeviceCaps(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        &caps
    );

    DWORD vertexProcessing = 0;
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//대체 무슨 그래픽카드를 쓰시는 겁니까...

    HRESULT result = m_d3d9->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        _hWnd,
        vertexProcessing | D3DCREATE_MULTITHREADED,
        &m_nowD3Dpp,
        &m_pDevice);

    switch (result)
    {
    case D3D_OK:
        //디바이스 생성에 성공
        break;
    case D3DERR_INVALIDCALL:
        //잘못된 인자
        break;
    case D3DERR_NOTAVAILABLE:
        //지원되지 않는 그래픽카드
        throw std::exception("다이렉트 x9을 지원하지 않습니다.");
        break;
    case D3DERR_OUTOFVIDEOMEMORY:
        //메모리 부족
        throw std::bad_alloc();
        break;
    default:
        break;
    }
}

cDevice::~cDevice()
{
    m_pDevice->Release();
    m_d3d9->Release();
}

std::vector<POINT> cDevice::GetDeviceSize()
{
    std::vector<POINT> vecSize;

    if (m_d3d9 == NULL)
    {
        return vecSize;
    }

    size_t size = m_d3d9->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
    vecSize.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        D3DDISPLAYMODE mode;
        POINT dissize;
        m_d3d9->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &mode);
        dissize.x = mode.Width;
        dissize.y = mode.Height;

        vecSize.push_back(dissize);
    }

    return vecSize;
}