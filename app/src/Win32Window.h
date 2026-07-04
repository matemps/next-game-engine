#pragma once

#include <Windows.h>


class Win32Window
{
public:
     // constructor

     Win32Window(
          const wchar_t* title,
          int width,
          int height
     );

     // destructor

     ~Win32Window();

public:
     // member functions

     bool ProcessMessages();

public:
     // inline functions

     inline HWND GetHWND() const { return m_HWND; }
     inline int GetWidth() const { return m_Width; }
     inline int GetHeight() const { return m_Height; }


private:
     static LRESULT CALLBACK WindowProc(
          HWND hwnd,
          UINT uMsg,
          WPARAM wParam,
          LPARAM lParam
     );

private:
     HWND m_HWND = nullptr;
     HINSTANCE m_HInstance = nullptr;

     const wchar_t* m_Title = nullptr;
     const wchar_t* m_ClassName = L"Win32WindowClass";

     int m_Width = 0;
     int m_Height = 0;
};