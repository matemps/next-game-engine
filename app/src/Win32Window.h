#pragma once

#include <Windows.h>
#include <functional>


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

     void SetTickFn(std::function<void()> callback);
     void SetOnDisplayChangeFn(std::function<void()> callback);
     void SetOnWindowMovedFn(std::function<void()> callback);
     void SetOnActivatedFn(std::function<void()> callback);
     void SetOnDeactivatedFn(std::function<void()> callback);
     void SetOnSuspendingFn(std::function<void()> callback);
     void SetOnResumingFn(std::function<void()> callback);
     void SetOnResizeFn(std::function<void(int, int)> callback);

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

     std::function<void()> TickFn;
     std::function<void()> OnDisplayChangeFn;
     std::function<void()> OnWindowMovedFn;
     std::function<void()> OnActivatedFn;
     std::function<void()> OnDeactivatedFn;
     std::function<void()> OnSuspendingFn;
     std::function<void()> OnResumingFn;
     std::function<void(int, int)> OnResizeFn;

     bool m_in_sizemove = false;
     bool m_in_suspend = false;
     bool m_minimized = false;
     bool m_fullscreen = false;
     // Set m_fullscreen to true if defaulting to fullscreen.
};