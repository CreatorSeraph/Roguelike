#pragma once

class cMainWindow
{
protected:
    wstring m_winClassName;
    wstring m_titleName;

    HWND m_hWnd;
public:
    cMainWindow();
    ~cMainWindow();

    bool Init(HINSTANCE instance, int cmdShow);
    WPARAM MainLoop(HACCEL _hAccel = nullptr);
public:
    LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    const wstring& GetWinClassName() { return m_winClassName; }
    const wstring& GetTitleName() { return m_titleName; }
};

 