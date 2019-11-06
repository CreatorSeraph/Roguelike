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

            //그래픽카드 lost상태일시 reset 시도
            g_Device.TryDeviceReset();

            //대충 여기서 메인게임 업데이트 돌아가야 한다는 뜻

            g_Device.Clear();
            if (SUCCEEDED(g_Device.BeginScene()))
            {
                //여기서 필요한 랜더링 함수 작동
                g_Device.EndScene();
            }

            if (FAILED(g_Device.Present()))
            {
                //device가 lost상태이거나 복구할수 없음
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