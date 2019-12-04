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
    std::filesystem::path path(L"./System/d3dpp.save");
    //파일을 저장하기 위한 폴더가 없으면
    if (auto parentPath = path.parent_path(); !std::filesystem::exists(parentPath))
    {
        //파을을 저장할 폴더를 작성해준다(폴더를 못만들 경우 false를 반환)
        if (!std::filesystem::create_directories(parentPath))
        {
            return false;
        }
    }
    std::wofstream d3dppData(path, std::ios::binary | std::ios::trunc);
    if (!d3dppData.is_open()) return false;

    d3dppData << m_nowD3Dpp.Windowed << m_nowD3Dpp.BackBufferWidth << m_nowD3Dpp.BackBufferHeight << m_nowD3Dpp.PresentationInterval;
    return true;
}

bool cDevice::LoadNowD3Dpp()
{
    std::wifstream d3dppData(L"./System/d3dpp.save", std::ios::binary);
    if (!d3dppData.is_open())
    {
        return false;
    }

    d3dppData >> m_nowD3Dpp.Windowed >> m_nowD3Dpp.BackBufferWidth >> m_nowD3Dpp.BackBufferHeight >> m_nowD3Dpp.PresentationInterval;
    return true;
}

cDevice::cDevice()
    : m_isInitialized(false), m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)), m_nowD3Dpp(), m_pDevice(nullptr)
    , m_clearColor(0xff0000ff)
    , m_isLost(false), m_needResetObj(false)
{
    if (!m_d3d9)
        wclog << "cDevice::cDevice - Can Not Call Direct3DCreate9" << endl;
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
    if (!m_d3d9)
        return E_FAIL;

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
        wclog << "cDevice::Init - Faild Load Last Setting" << endl;
        //함수 인자가 잘못되었다.
        //최근에 불러온 설정이 있었다면, 기본설정으로 초기화 해본다.
        m_nowD3Dpp = MakeDefaultD3Dpp(_hWnd);
        result = CreateDevice();
    }
    
    return result;
}

HRESULT cDevice::Clear()
{
    return m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, m_clearColor, 1.0f, 0);
}

HRESULT cDevice::BeginScene()
{
    return m_pDevice->BeginScene();
}

HRESULT cDevice::EndScene()
{
    return m_pDevice->EndScene();
}

HRESULT cDevice::Present()
{
    auto result = m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST)
    {
        //lost가 일어난 상황의 경우는 절전모드등의 이유로 cpu점유를 가져오지 못 할 수도 있으므로
        //여기서는 lost임을 체크만 해주고, tryDeviceReset에서 처리한다.
        m_isLost = true;
    }

    return result;
}

HRESULT cDevice::TryDeviceReset()
{
    //와 여기가 진짜 헬이네
    HRESULT result = m_pDevice->TestCooperativeLevel();
    if (result == D3DERR_DEVICENOTRESET)
    {
        //여러번 reset을 시도하는 일이 없도록 함.
        if (m_needResetObj)
        {
            //onLost
            if (m_lostDeviceFunc)
                m_lostDeviceFunc();

            m_needResetObj = false;
        }

        //reset
        result = m_pDevice->Reset(&m_nowD3Dpp);
        if (FAILED(result))
            return result;

        //onReset
        if (m_resetDeviceFunc)
            m_resetDeviceFunc();

        //오브젝트가 reset되었으므로, 다음 lost시에는 다시 reset해야함
        m_needResetObj = true;
        m_isLost = false;
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