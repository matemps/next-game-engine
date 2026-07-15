#include <string>

#include "Application.h"
#include "Win32Window.h"
#include "Game.h"
#include "Keyboard.h"
#include "../../timer/StepTimer.h"


using namespace DirectX;
using namespace DX;


// constructor
Application::Application(Game* game) : m_Game(game)
{
    std::string title = game->GetTitle();
    std::wstring wideTitle(title.begin(), title.end());
    int width = game->GetWidth();
    int height = game->GetHeight();

    m_Window = new Win32Window();
    m_Window->Init(wideTitle.c_str(), width, height);

    m_DeviceResources = new DX::DeviceResources();
    m_DeviceResources->SetWindow(m_Window->GetHWND(), width, height);
    m_DeviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();
    m_DeviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_Window->SetTickFn([this]() { Tick(); });
    m_Window->SetOnDisplayChangeFn([this]() { OnDisplayChange(); });
    m_Window->SetOnWindowMovedFn([this]() { OnWindowMoved(); });
    m_Window->SetOnActivatedFn([this]() { OnActivated(); });
    m_Window->SetOnDeactivatedFn([this]() { OnDeactivated(); });
    m_Window->SetOnSuspendingFn([this]() { OnSuspending(); });
    m_Window->SetOnResumingFn([this]() { OnResuming(); });
    m_Window->SetOnResizeFn([this](int w, int h) { OnResize(w, h); });

    m_keyboard = std::make_unique<Keyboard>();  // init input

    game->Init();   // init game
}

// Executes the basic game loop.
void Application::Run()
{
    while (m_Window->ProcessMessages())
    {
        Tick();
    }
}

// Advances and renders a single frame.
void Application::Tick()
{
    m_Timer.Tick([&]()
    {
        Update(m_Timer);
    });

    Render();
}

// Updates the world.
void Application::Update(StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    m_Game->Update(elapsedTime);
}


// Draws the scene.
void Application::Render()
{
    // Don't try to render anything before the first Update.
    if (m_Timer.GetFrameCount() == 0) return;

    Clear();

    m_DeviceResources->PIXBeginEvent(L"Render");
    auto context = m_DeviceResources->GetD3DDeviceContext();

    m_Game->Render();

    m_DeviceResources->PIXEndEvent();

    // Show the new frame.
    m_DeviceResources->Present();
}

// Helper method to clear the back buffers.
void Application::Clear()
{
    m_DeviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_DeviceResources->GetD3DDeviceContext();
    auto renderTarget = m_DeviceResources->GetRenderTargetView();
    auto depthStencil = m_DeviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    const auto viewport = m_DeviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_DeviceResources->PIXEndEvent();
}

void Application::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Application::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Application::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Application::OnResuming()
{
    m_Timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Application::OnWindowMoved()
{
    const auto r = m_DeviceResources->GetOutputSize();
    m_DeviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Application::OnDisplayChange()
{
    m_DeviceResources->UpdateColorSpace();
}

void Application::OnResize(int width, int height)
{
    if (!m_DeviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// These are the resources that depend on the device.
void Application::CreateDeviceDependentResources()
{
    auto device = m_DeviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Application::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Application::OnDeviceLost()
{
    // TO DO: Add Direct3D resource cleanup here.
}

void Application::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}

// destructor
Application::~Application()
{
    if (m_Game)
    {
        m_Game->Shutdown();

        delete m_Game;
    }

    delete m_DeviceResources;
    delete m_Window;
}