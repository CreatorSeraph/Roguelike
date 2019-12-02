#include "pch.h"
#include "cMainWindow.h"
#include "resource.h"

#include "Object/cObject.h"

#include <thread>

cMainWindow::cMainWindow()
    : m_winClassName(L"RogueLike"), m_titleName(L"(���� �����̶�� ��)"), m_hWnd(nullptr)
    , m_isActive(true)
{
}

cMainWindow::~cMainWindow()
{
}

bool cMainWindow::Init(HINSTANCE instance, int cmdShow)
{
    DWORD dwStyle = WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;
    m_hWnd = CreateWindowExW(0, m_winClassName.c_str(), m_titleName.c_str(), dwStyle,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instance, nullptr);

    if (!m_hWnd)
    {
        return false;
    }

    ShowWindow(m_hWnd, cmdShow);
    UpdateWindow(m_hWnd);

    return SUCCEEDED(g_Device.Init(m_hWnd));
}

WPARAM cMainWindow::MainLoop(HACCEL _hAccel)
{
    g_Timer.Reset();
    g_Timer.SetFPSLimit(60);

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
        {
            if (!TranslateAccelerator(m_hWnd, _hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (g_Device.isLostDevice() || !m_isActive)
            {
                //ȭ���� ��Ȱ��ȭ �Ǿ�������, cpu�������� ������
                std::this_thread::sleep_for(50ms);
            }

            //�׷���ī�� lost�����Ͻ� reset �õ�
            if (g_Device.isLostDevice())
            {
                auto resetResult = g_Device.TryDeviceReset();
                //����̽��� lost���°� ����(�ٸ� ���α׷��� Device�� ������̰ų�, ���������� ������ ������� ����.
                if (resetResult == D3DERR_DEVICELOST)
                    continue;
                //Device�� ������ ��������
                else if (resetResult != D3D_OK)
                {
                    SendMessage(m_hWnd, WM_CLOSE, 0, 0);
                    continue;
                }
            }

            //���� ���⼭ ���ΰ��� ������Ʈ ���ư��� �Ѵٴ� ��
            wclog << g_Timer.Update() << std::endl;

            g_Device.Clear();
            if (SUCCEEDED(g_Device.BeginScene()))
            {
                //���⼭ �ʿ��� ������ �Լ� �۵�
                g_Device.EndScene();
            }

            if (FAILED(g_Device.Present()))
            {
                //device�� lost�����̰ų� �����Ҽ� ����
            }
        }
    }

    return msg.wParam;
}

LRESULT cMainWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ACTIVATEAPP:
        if (wParam && !m_isActive)
        {
            m_isActive = true;
            //���� Ŀ�� �ٽ� ��ſ�
            //���� �ٽ� Ű���� ��ſ�
        }
        else if(!wParam && m_isActive)
        {
            m_isActive = false;
            //���� Ŀ�� �������
            //���� Ŀ�� ��� Ǯ�����
            //���� ��ɴ� ������Ű�� Ű���� �������
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}