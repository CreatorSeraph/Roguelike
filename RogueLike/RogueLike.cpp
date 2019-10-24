﻿#include "pch.h"
#include "framework.h"
#include "RogueLike.h"

#include "Log/consoleLog.h"
#include "Window/cMainWindow.h"

//주석처리시, 콘솔 로그를 띄우지 않음.
#define USE_CONSOLELOG

cMainWindow g_mainWindow;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_mainWindow.MsgProc(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROGUELIKE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ROGUELIKE);
    wcex.lpszClassName = g_mainWindow.GetWinClassName().c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef USE_CONSOLELOG
    consoleLog log;
#endif
    MyRegisterClass(hInstance);

    if (!g_mainWindow.Init(hInstance, nCmdShow))
        return false;

    auto result = g_mainWindow.MainLoop();

    return result;
}