#include "pch.h"
#include "cMainWindow.h"
#include "resource.h"

cMainWindow::cMainWindow()
    : m_winClassName(L"RogueLike"), m_titleName(L"(대충 제목이라는 뜻)"), m_hWnd(nullptr)
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

    return true;
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
            //g_Timer.Update();
            wclog << g_Timer.Update() << std::endl;
        }
    }

    return msg.wParam;
}

LRESULT cMainWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}