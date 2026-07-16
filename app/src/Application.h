#pragma once

#include "Game.h"
#include "Win32Window.h"
#include "../../graphics/DeviceResources.h"
#include "../../timer/StepTimer.h"
#include "Keyboard.h"


class Application : public DX::IDeviceNotify
{
    public:
        // constructor
        Application(Game* game);

        // destructor
        ~Application();

    
    public:
        // member functions

        void Run();
        void Tick();
        void Update(StepTimer const& timer);
        void Render();
        void Clear();
        
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();


    public:
        // Messages

        void OnActivated();
        void OnDeactivated();
        void OnSuspending();
        void OnResuming();
        void OnWindowMoved();
        void OnDisplayChange();
        void OnResize(int width, int height);


    public:
        // IDeviceNotify

        void OnDeviceLost() override;
        void OnDeviceRestored() override;
    

    private:
        Game* m_Game = nullptr;
        Win32Window* m_Window = nullptr;
        DX::DeviceResources* m_DeviceResources = nullptr;
        StepTimer m_Timer = StepTimer();
        DirectX::Keyboard* m_keyboard = nullptr;
};