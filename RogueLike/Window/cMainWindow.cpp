#include "pch.h"
#include "cMainWindow.h"
#include "resource.h"

#include "Object/cObject.h"

#include <thread>

cMainWindow::cMainWindow()
    : m_winClassName(L"RogueLike"), m_titleName(L"(대충 제목이라는 뜻)"), m_hWnd(nullptr)
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
                //화면이 비활성화 되어있을시, cpu점유율을 낮춰줌
                std::this_thread::sleep_for(50ms);
            }

            //그래픽카드 lost상태일시 reset 시도
            if (g_Device.isLostDevice())
            {
                auto resetResult = g_Device.TryDeviceReset();
                //디바이스의 lost상태가 유지(다른 프로그램이 Device를 사용중이거나, 절전모드등의 이유로 사용하지 못함.
                if (resetResult == D3DERR_DEVICELOST)
                    continue;
                //Device의 복구에 실패했음
                else if (resetResult != D3D_OK)
                {
                    SendMessage(m_hWnd, WM_CLOSE, 0, 0);
                    continue;
                }
            }

            //대충 여기서 메인게임 업데이트 돌아가야 한다는 뜻
            wclog << g_Timer.Update() << std::endl;

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
    case WM_ACTIVATEAPP:
        if (wParam && !m_isActive)
        {
            m_isActive = true;
            //뭐야 커서 다시 잠궈요
            //뭐야 다시 키셋팅 잠궈요
        }
        else if(!wParam && m_isActive)
        {
            m_isActive = false;
            //뭐야 커서 돌려줘요
            //뭐야 커서 잠금 풀어줘요
            //뭐야 잠궜던 윈도우키랑 키셋팅 돌려줘요
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}