#include "Win32Window.h"
#include "../../input/Input.h"

#include <stdexcept>


// constructor
Win32Window::Win32Window(
     const wchar_t* title,
     int width,
     int height
) :
     m_Title(title),
     m_Width(width),
     m_Height(height)
{
     m_HInstance = GetModuleHandleW(nullptr);

     WNDCLASSEXW wc = {};
     wc.cbSize = sizeof(WNDCLASSEXW);
     wc.lpfnWndProc = WindowProc;
     wc.hInstance = m_HInstance;
     wc.lpszClassName = m_ClassName;
     wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);

     RegisterClassExW(&wc);

     RECT rect = { 0, 0, width, height };

     AdjustWindowRect(
          &rect,
          WS_OVERLAPPEDWINDOW,
          FALSE
     );

     m_HWND = CreateWindowExW(
          0,
          m_ClassName,
          m_Title,
          WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT,
          CW_USEDEFAULT,
          rect.right - rect.left,
          rect.bottom - rect.top,
          nullptr,
          nullptr,
          m_HInstance,
          nullptr
     );

     if (!m_HWND)
     {
          throw std::runtime_error("Failed to create window.");
     }

     ShowWindow(m_HWND, SW_SHOW);
}


bool Win32Window::ProcessMessages()
{
     MSG msg = {};

     while (PeekMessageW(
          &msg,
          nullptr,
          0,
          0,
          PM_REMOVE
     ))
     {
          if (msg.message == WM_QUIT)
          {
               return false;
          }

          TranslateMessage(&msg);
          DispatchMessageW(&msg);
     }

     return true;
}


LRESULT CALLBACK Win32Window::WindowProc(
     HWND hwnd,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam
)
{
     switch (uMsg)
     {
          case WM_DESTROY:
          {
               PostQuitMessage(0);
               return 0;
          }
          case WM_CLOSE:
          {
               DestroyWindow(hwnd);
               return 0;
          }
          case WM_KEYDOWN:
          {
               Input::SetKeyDown(wParam);
               return 0;
          }
          case WM_KEYUP:
          {
               Input::SetKeyUp(wParam);
               return 0;
          }
     }

     return DefWindowProcW(
          hwnd,
          uMsg,
          wParam,
          lParam
     );
}


// destructor
Win32Window::~Win32Window()
{
     if (m_HWND)
          DestroyWindow(m_HWND);

     UnregisterClassW(m_ClassName, m_HInstance);
}