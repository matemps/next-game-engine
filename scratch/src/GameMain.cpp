#include <string>
#include <iostream>

#include "Game.h"
#include "GameAPI.h"

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "Camera.h"
#include "World.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


class Scratch : public Game
{
    public:
        Scratch() : Game("Sample Game", 1280, 720) {}
    
    public:
        void Init() override
        {
            std::cout << "Game initialized." << std::endl;

            m_World = new World();
            m_World->AddBlock(100.0f, 100.0f, 250.0f);
            m_World->AddPlane(1000.0f, 1000.0f, Matrix::CreateTranslation(0.0f, -150.0f, 0.0f));
        }

        void Update(float dt) override
        {
            Keyboard::State kbState = GetKeyboard()->GetState();

            if (kbState.W) { m_Camera->MoveForward(dt); }
            if (kbState.S) { m_Camera->MoveBackward(dt); }
            if (kbState.A) { m_Camera->MoveLeft(dt); }
            if (kbState.D) { m_Camera->MoveRight(dt); }

            if (kbState.Space) { m_Camera->MoveUp(dt); }
            if (kbState.LeftControl) { m_Camera->MoveDown(dt); }

            if (kbState.Left) { m_Camera->RotateLeft(dt); }
            if (kbState.Right) { m_Camera->RotateRight(dt); }
            if (kbState.Up) { m_Camera->RotateUp(dt); }
            if (kbState.Down) { m_Camera->RotateDown(dt); }
        }

        void Render() override
        {
            m_World->Render(
                GetGameRenderer(),
                m_Camera->GetViewMatrix(),
                m_Camera->GetProjectionMatrix()
            );
        }

        void Shutdown() override
        {
            std::cout << "Game shutting down." << std::endl;
        }


        void OnActivated() override {}
        void OnDeactivated() override {}
        void OnSuspending() override {}
        void OnResuming() override {}

        // Runs when the window is created or when the window is resized.
        void CreateWindowSizeDependentResources() override
        {
            // TO DO: protect against a divide by zero exception
            float aspectRatio = static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());

            m_Camera->SetAspectRatio(aspectRatio);
        }

    private:
        Camera* m_Camera = new Camera();
        World* m_World = nullptr;
};

EXPORT_GAME(Scratch);
