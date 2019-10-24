#include "pch.h"
#include "cMainWindow.h"
#include "resource.h"
#include "Timer/cGameTimer.h"

cMainWindow::cMainWindow()
    : m_winClassName(L"RogueLike"), m_titleName(L"(대충 제목이라는 뜻)")
{
}

cMainWindow::~cMainWindow()
{
}

bool cMainWindow::Init(HINSTANCE instance, int cmdShow)
{
    DWORD dwStyle = WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;
    HWND hWnd = CreateWindowExW(0, m_winClassName.c_str(), m_titleName.c_str(), dwStyle,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instance, nullptr);

    if (!hWnd)
    {
        return false;
    }

    ShowWindow(hWnd, cmdShow);
    UpdateWindow(hWnd);

    return true;
}

WPARAM cMainWindow::MainLoop()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    g_Timer.SetFPSLimit(0);

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
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
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}