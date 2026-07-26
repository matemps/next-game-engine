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
            m_World->AddCube(1.0f);
            m_World->AddPlane(5.0f, 5.0f, Matrix::CreateTranslation(0.0f, -0.5f, 0.0f));
        }

        void Update(float dt) override
        {
            Keyboard::State kbState = GetKeyboard()->GetState();

            float moveAmount = m_MoveSpeed * dt;
            float rotateAmount = m_RotateSpeed * dt;

            if (kbState.W) { m_Camera->MoveForward(moveAmount); }
            if (kbState.S) { m_Camera->MoveForward(-moveAmount); }
            if (kbState.A) { m_Camera->MoveRight(-moveAmount); }
            if (kbState.D) { m_Camera->MoveRight(moveAmount); }

            if (kbState.Space) { m_Camera->MoveUp(moveAmount); }
            if (kbState.LeftControl) { m_Camera->MoveUp(-moveAmount); }

            float yawDelta = 0.0f;
            float pitchDelta = 0.0f;

            if (kbState.Left) { yawDelta -= rotateAmount; }
            if (kbState.Right) { yawDelta += rotateAmount; }
            if (kbState.Up) { pitchDelta += rotateAmount; }
            if (kbState.Down) { pitchDelta -= rotateAmount; }

            if (yawDelta != 0.0f || pitchDelta != 0.0f) { m_Camera->Rotate(yawDelta, pitchDelta); }
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
        Camera* m_Camera = new Camera(Vector3(0.0f, 0.7f, 1.5f), Vector3(0.0f, -0.1f, 0.0f));
        World* m_World = nullptr;

        float m_MoveSpeed = 2.0f;   // units per second
        float m_RotateSpeed = XM_PI / 2.0f; // radians per second
};

EXPORT_GAME(Scratch);
