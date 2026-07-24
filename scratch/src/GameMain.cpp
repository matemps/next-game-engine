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
#include "GameSphere.h"


using namespace DirectX;


class Scratch : public Game
{
    public:
        Scratch() : Game("Sample Game", 1280, 720) {}
    
    public:
        void Init() override
        {
            std::cout << "Game initialized." << std::endl;

            m_GameCube = new GameCube(1.0f);
            m_GameSphere = new GameSphere(0.5f);
        }

        void Update(float dt) override
        {
            Vector3 eye(0.0f, 0.7f, 3.0f);
            Vector3 at(0.0f, -0.1f, 0.0f);
            Matrix view = Matrix::CreateLookAt(eye, at, Vector3::UnitY);
            m_WVP->View = view;
            m_SphereWVP->View = view;

            m_RotationAngle += dt * XM_PIDIV4;
            m_WVP->World = Matrix::CreateRotationY(m_RotationAngle);

            m_SphereRotationAngle += dt * XM_PIDIV2;
            m_SphereWVP->World =
                Matrix::CreateRotationY(m_SphereRotationAngle) *
                Matrix::CreateTranslation(2.0f, 0.0f, 0.0f);


            Keyboard::State kbState = GetKeyboard()->GetState();

            if (kbState.W) { std::cout << "Pressed W." << std::endl; }
            if (kbState.A) { std::cout << "Pressed A." << std::endl; }
            if (kbState.S) { std::cout << "Pressed S." << std::endl; }
            if (kbState.D) { std::cout << "Pressed D." << std::endl; }
        }

        void Render() override
        {
            GetGameRenderer()->DrawCube(m_GameCube, m_WVP);
            GetGameRenderer()->DrawSphere(m_GameSphere, m_SphereWVP);
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
            Matrix projection = Matrix::CreatePerspectiveFieldOfView(
                fovAngleY,
                aspectRatio,
                0.01f,
                100.0f
            );
            m_WVP->Projection = projection;
            m_SphereWVP->Projection = projection;
        }

    private:
        WVP* m_WVP = new WVP();
        WVP* m_SphereWVP = new WVP();

        GameCube* m_GameCube = nullptr;
        GameSphere* m_GameSphere = nullptr;

        float m_RotationAngle = 0.0f;
        float m_SphereRotationAngle = 0.0f;
};

EXPORT_GAME(Scratch);
