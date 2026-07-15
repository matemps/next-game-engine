#include "Win32Window.h"
#include "Keyboard.h"
#include <stdexcept>


void Win32Window::Init(
     const wchar_t* title,
     int width,
     int height
)
{
     m_Title = title;
     m_Width = width;
     m_Height = height;

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

          // pass pointer of this win32window instance to lpParam.
          this
     );

     // When the window procedure handles the WM_NCCREATE or WM_CREATE message,
     // it will extract our win32window instance pointer from the message data.


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


// setters

void Win32Window::SetTickFn(std::function<void()> callback)
{
     TickFn = callback;
}
void Win32Window::SetOnDisplayChangeFn(std::function<void()> callback)
{
     OnDisplayChangeFn = callback;
}
void Win32Window::SetOnWindowMovedFn(std::function<void()> callback)
{
     OnWindowMovedFn = callback;
}
void Win32Window::SetOnActivatedFn(std::function<void()> callback)
{
     OnActivatedFn = callback;
}
void Win32Window::SetOnDeactivatedFn(std::function<void()> callback)
{
     OnDeactivatedFn = callback;
}
void Win32Window::SetOnSuspendingFn(std::function<void()> callback)
{
     OnSuspendingFn = callback;
}
void Win32Window::SetOnResumingFn(std::function<void()> callback)
{
     OnResumingFn = callback;
}
void Win32Window::SetOnResizeFn(std::function<void(int, int)> callback)
{
     OnResizeFn = callback;
}


LRESULT CALLBACK Win32Window::WindowProc(
     HWND hwnd,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam
)
{
     Win32Window* pWindow = nullptr;

     // CreateWindowsEx function sends WM_NCCREATE message.
     if (uMsg == WM_NCCREATE)
     {
          // Extract the CREATESTRUCTW structure from lParam.
          CREATESTRUCTW *pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

          // Retrieve pointer to this Win32Window instance.
          pWindow = reinterpret_cast<Win32Window*>(pCreate->lpCreateParams);

          // Store the pointer in the instance data for the window.
          SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);
     }
     else
     {
          // Retrieve the stored pointer.

          LONG_PTR ptr = GetWindowLongPtrW(hwnd, GWLP_USERDATA);
          pWindow = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
     }

     if (pWindow)
     {
          switch (uMsg)
          {
               case WM_PAINT:
               {
                    if (pWindow->m_in_sizemove && pWindow->TickFn)
                    {
                         pWindow->TickFn();
                    }
                    else
                    {
                         PAINTSTRUCT ps;
                         std::ignore = BeginPaint(hwnd, &ps);
                         EndPaint(hwnd, &ps);
                    }

                    break;
               }

               case WM_DISPLAYCHANGE:
               {
                    if (pWindow->OnDisplayChangeFn)
                    {
                         pWindow->OnDisplayChangeFn();
                    }
                    break;
               }

               case WM_MOVE:
               {
                    if (pWindow->OnWindowMovedFn)
                    {
                         pWindow->OnWindowMovedFn();
                    }
                    break;
               }

               case WM_ENTERSIZEMOVE:
               {
                    pWindow->m_in_sizemove = true;
                    break;
               }

               case WM_EXITSIZEMOVE:
               {
                    pWindow->m_in_sizemove = false;

                    if (pWindow->OnResizeFn)
                    {
                         RECT rc;
                         GetClientRect(hwnd, &rc);

                         pWindow->OnResizeFn(rc.right - rc.left, rc.bottom - rc.top);
                    }
                    break;
               }

               case WM_GETMINMAXINFO:
               {
                    if (lParam)
                    {
                         auto info = reinterpret_cast<MINMAXINFO*>(lParam);
                         info->ptMinTrackSize.x = 320;
                         info->ptMinTrackSize.y = 200;
                    }
                    break;
               }

               case WM_ACTIVATEAPP:
               {
                    if (wParam && pWindow->OnActivatedFn)
                    {
                         pWindow->OnActivatedFn();
                    }
                    else if (pWindow->OnDeactivatedFn)
                    {
                         pWindow->OnDeactivatedFn();
                    }

                    break;
               }

               case WM_POWERBROADCAST:
               {
                    switch (wParam)
                    {
                         case PBT_APMQUERYSUSPEND:
                         {
                              if (!pWindow->m_in_suspend && pWindow->OnSuspendingFn)
                                   pWindow->OnSuspendingFn();

                              pWindow->m_in_suspend = true;
                              return TRUE;
                         }

                         case PBT_APMRESUMESUSPEND:
                         {
                              if (!pWindow->m_minimized)
                              {
                                   if (pWindow->m_in_suspend && pWindow->OnResumingFn)
                                        pWindow->OnResumingFn();

                                   pWindow->m_in_suspend = false;
                              }

                              return TRUE;
                         }

                         default:
                              break;
                    }

                    break;
               }

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
               }
               case WM_KEYUP:
               {
               }

               case WM_SYSKEYUP:
               {
                    DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
                    break;
               }

               case WM_SYSKEYDOWN:
               {
                    DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
                    break;
               }

               case WM_SIZE:
               {
                    if (wParam == SIZE_MINIMIZED)
                    {
                         if (!pWindow->m_minimized)
                         {
                              pWindow->m_minimized = true;

                              if (!pWindow->m_in_suspend && pWindow->OnSuspendingFn)
                                   pWindow->OnSuspendingFn();

                              pWindow->m_in_suspend = true;
                         }
                    }
                    else if (pWindow->m_minimized)
                    {
                         pWindow->m_minimized = false;

                         if (pWindow->m_in_suspend && pWindow->OnResumingFn)
                              pWindow->OnResumingFn();

                         pWindow->m_in_suspend = false;
                    }
                    else if (!pWindow->m_in_sizemove)
                    {
                         if (pWindow->OnResizeFn)
                              pWindow->OnResizeFn(pWindow->m_Width, pWindow->m_Height);
                    }

                    break;
               }
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