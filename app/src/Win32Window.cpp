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
          this
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


void Win32Window::SetTickFn(std::function<void()> callback)
{
     TickFn = std::move(callback);
}
void Win32Window::SetOnDisplayChangeFn(std::function<void()> callback)
{
     OnDisplayChangeFn = std::move(callback);
}
void Win32Window::SetOnWindowMovedFn(std::function<void()> callback)
{
     OnWindowMovedFn = std::move(callback);
}
void Win32Window::SetOnActivatedFn(std::function<void()> callback)
{
     OnActivatedFn = std::move(callback);
}
void Win32Window::SetOnDeactivatedFn(std::function<void()> callback)
{
     OnDeactivatedFn = std::move(callback);
}
void Win32Window::SetOnSuspendingFn(std::function<void()> callback)
{
     OnSuspendingFn = std::move(callback);
}
void Win32Window::SetOnResumingFn(std::function<void()> callback)
{
     OnResumingFn = std::move(callback);
}
void Win32Window::SetOnResizeFn(std::function<void(int, int)> callback)
{
     OnResizeFn = std::move(callback);
}


LRESULT CALLBACK Win32Window::WindowProc(
     HWND hwnd,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam
)
{
     Win32Window* window = nullptr;

     if (uMsg == WM_NCCREATE)
     {
          auto createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
          window = static_cast<Win32Window*>(createStruct->lpCreateParams);

          SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
     }
     else
     {
          window = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
     }

     if (window)
     {
          switch (uMsg)
          {
               case WM_PAINT:
               {
                    if (window->m_in_sizemove && window->TickFn)
                    {
                         window->TickFn();
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
                    if (window->OnDisplayChangeFn)
                    {
                         window->OnDisplayChangeFn();
                    }
                    break;
               }

               case WM_MOVE:
               {
                    if (window->OnWindowMovedFn)
                    {
                         window->OnWindowMovedFn();
                    }
                    break;
               }

               case WM_ENTERSIZEMOVE:
               {
                    window->m_in_sizemove = true;
                    break;
               }

               case WM_EXITSIZEMOVE:
               {
                    window->m_in_sizemove = false;

                    if (window->OnResizeFn)
                    {
                         RECT rc;
                         GetClientRect(hwnd, &rc);

                         window->OnResizeFn(rc.right - rc.left, rc.bottom - rc.top);
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
                    if (wParam && window->OnActivatedFn)
                    {
                         window->OnActivatedFn();
                    }
                    else if (window->OnDeactivatedFn)
                    {
                         window->OnDeactivatedFn();
                    }

                    break;
               }

               case WM_POWERBROADCAST:
               {
                    switch (wParam)
                    {
                         case PBT_APMQUERYSUSPEND:
                         {
                              if (!window->m_in_suspend && window->OnSuspendingFn)
                                   window->OnSuspendingFn();

                              window->m_in_suspend = true;
                              return TRUE;
                         }

                         case PBT_APMRESUMESUSPEND:
                         {
                              if (!window->m_minimized)
                              {
                                   if (window->m_in_suspend && window->OnResumingFn)
                                        window->OnResumingFn();

                                   window->m_in_suspend = false;
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
                    Input::SetKeyDown(wParam);
                    return 0;
               }
               case WM_KEYUP:
               {
                    Input::SetKeyUp(wParam);
                    return 0;
               }

               case WM_SYSKEYUP:
               {
               }

               case WM_SYSKEYDOWN:
               {
               }

               case WM_SIZE:
               {
                    if (wParam == SIZE_MINIMIZED)
                    {
                         if (!window->m_minimized)
                         {
                              window->m_minimized = true;

                              if (!window->m_in_suspend && window->OnSuspendingFn)
                                   window->OnSuspendingFn();

                              window->m_in_suspend = true;
                         }
                    }
                    else if (window->m_minimized)
                    {
                         window->m_minimized = false;

                         if (window->m_in_suspend && window->OnResumingFn)
                              window->OnResumingFn();

                         window->m_in_suspend = false;
                    }
                    else if (!window->m_in_sizemove)
                    {
                         if (window->OnResizeFn)
                              window->OnResizeFn(window->m_Width, window->m_Height);
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