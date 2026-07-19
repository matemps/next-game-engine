#include <string>
#include <iostream>

#include "Game.h"
#include "GameAPI.h"

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "WVP.h"

#include "GameCube.h"


using namespace DirectX;


class Scratch : public Game
{
    public:
        Scratch() : Game("Sample Game", 1280, 720) {}
    
    public:
        void Init() override
        {
            std::cout << "Game initialized." << std::endl;
            
            Vertex vertices[] =
            {
                { Vector3(-0.5f, -0.5f, -0.5f), Color(0.0f, 0.0f, 0.0f) },
                { Vector3(-0.5f, -0.5f,  0.5f), Color(0.0f, 0.0f, 1.0f) },
                { Vector3(-0.5f,  0.5f, -0.5f), Color(0.0f, 1.0f, 0.0f) },
                { Vector3(-0.5f,  0.5f,  0.5f), Color(0.0f, 1.0f, 1.0f) },
                { Vector3( 0.5f, -0.5f, -0.5f), Color(1.0f, 0.0f, 0.0f) },
                { Vector3( 0.5f, -0.5f,  0.5f), Color(1.0f, 0.0f, 1.0f) },
                { Vector3( 0.5f,  0.5f, -0.5f), Color(1.0f, 1.0f, 0.0f) },
                { Vector3( 0.5f,  0.5f,  0.5f), Color(1.0f, 1.0f, 1.0f) },
            };
            uint16_t indices[] =
            {
                0,2,1, 1,2,3,
                4,5,6, 5,7,6,
                0,1,5, 0,5,4,
                2,6,7, 2,7,3,
                0,4,6, 0,6,2,
                1,3,7, 1,7,5,
            };
            m_GameCube = new GameCube(vertices, indices);
        }
        
        void Update(float dt) override
        {
            Vector3 eye(0.0f, 0.7f, 1.5f);
            Vector3 at(0.0f, -0.1f, 0.0f);
            m_WVP->View = Matrix::CreateLookAt(eye, at, Vector3::UnitY);

            m_RotationAngle += dt * XM_PIDIV4;
            m_WVP->World = Matrix::CreateRotationY(m_RotationAngle);


            Keyboard::State kbState = GetKeyboard()->GetState();

            if (kbState.W) { std::cout << "Pressed W." << std::endl; }
            if (kbState.A) { std::cout << "Pressed A." << std::endl; }
            if (kbState.S) { std::cout << "Pressed S." << std::endl; }
            if (kbState.D) { std::cout << "Pressed D." << std::endl; }
        }

        void Render() override
        {
            GetGameRenderer()->DrawCube(m_GameCube, m_WVP);
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

            float fovAngleY = 70.0f * XM_PI / 180.0f;

            // This is a simple example of change that can be made when the app is in
            // portrait or snapped view.
            if (aspectRatio < 1.0f)
            {
                fovAngleY *= 2.0f;
            }

            // This sample makes use of a right-handed coordinate system using row-major matrices.
            m_WVP->Projection = Matrix::CreatePerspectiveFieldOfView(
                fovAngleY,
                aspectRatio,
                0.01f,
                100.0f
            );
        }

    private:
        WVP* m_WVP = new WVP();

        GameCube* m_GameCube = nullptr;
        
        float m_RotationAngle = 0.0f;
};

EXPORT_GAME(Scratch);
