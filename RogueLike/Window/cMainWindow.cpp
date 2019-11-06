#include "pch.h"
#include "cMainWindow.h"
#include "resource.h"

cMainWindow::cMainWindow()
    : m_winClassName(L"RogueLike"), m_titleName(L"(���� �����̶�� ��)"), m_hWnd(nullptr)
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
            wclog << g_Timer.Update() << std::endl;

            //�׷���ī�� lost�����Ͻ� reset �õ�
            g_Device.TryDeviceReset();

            //���� ���⼭ ���ΰ��� ������Ʈ ���ư��� �Ѵٴ� ��

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
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}