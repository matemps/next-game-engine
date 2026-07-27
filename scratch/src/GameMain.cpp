#include <string>
#include <iostream>

#include "Game.h"
#include "GameAPI.h"

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "World.h"
#include "Player.h"


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
            m_World->AddPlane(1000.0f, 1000.0f, Matrix::CreateTranslation(0.0f, -250.0f, 0.0f));

            m_Player = new Player(m_World->GetPhysicsWorld(), Vector3(0.0f, 400.0f, 0.0f));
        }

        void Update(float dt) override
        {
            Keyboard::State kbState = GetKeyboard()->GetState();

            m_Player->Update(kbState, dt);
            m_World->Step(dt);
        }

        void Render() override
        {
            m_World->Render(
                GetGameRenderer(),
                m_Player->GetPlayerCamera()->GetViewMatrix(),
                m_Player->GetPlayerCamera()->GetProjectionMatrix()
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

            m_Player->GetPlayerCamera()->SetAspectRatio(aspectRatio);
        }

    private:
        World* m_World = nullptr;
        Player* m_Player = nullptr;
};

EXPORT_GAME(Scratch);
