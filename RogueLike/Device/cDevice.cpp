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
    //������ ����� ���缭 ����
    d3dpp.BackBufferWidth = 0;
    d3dpp.BackBufferHeight = 0;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.hDeviceWindow = _hWnd;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    //��������ȭ on
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    //â��� ����
    d3dpp.Windowed = true;

    return d3dpp;
}

bool cDevice::SaveNowD3Dpp()
{
    std::filesystem::path path(L"./System/d3dpp.save");
    //������ �����ϱ� ���� ������ ������
    if (auto parentPath = path.parent_path(); !std::filesystem::exists(parentPath))
    {
        //������ ������ ������ �ۼ����ش�(������ ������ ��� false�� ��ȯ)
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
        vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//��ü ���� �׷���ī�带 ���ô� �̴ϱ�...

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
        //�Լ� ���ڰ� �߸��Ǿ���.
        //�ֱٿ� �ҷ��� ������ �־��ٸ�, �⺻�������� �ʱ�ȭ �غ���.
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
        //lost�� �Ͼ ��Ȳ�� ���� ���������� ������ cpu������ �������� �� �� ���� �����Ƿ�
        //���⼭�� lost���� üũ�� ���ְ�, tryDeviceReset���� ó���Ѵ�.
        m_isLost = true;
    }

    return result;
}

HRESULT cDevice::TryDeviceReset()
{
    //�� ���Ⱑ ��¥ ���̳�
    HRESULT result = m_pDevice->TestCooperativeLevel();
    if (result == D3DERR_DEVICENOTRESET)
    {
        //������ reset�� �õ��ϴ� ���� ������ ��.
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

        //������Ʈ�� reset�Ǿ����Ƿ�, ���� lost�ÿ��� �ٽ� reset�ؾ���
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