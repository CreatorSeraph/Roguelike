#pragma once

class cMainWindow
{
protected:
    wstring m_winClassName;
    wstring m_titleName;
public:
    cMainWindow();
    ~cMainWindow();

    bool Init(HINSTANCE instance, int cmdShow);
    WPARAM MainLoop();
public:
    LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    const wstring& GetWinClassName() { return m_winClassName; }
    const wstring& GetTitleName() { return m_titleName; }
};

 