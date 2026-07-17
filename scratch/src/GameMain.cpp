#include <string>
#include <iostream>

#include "Game.h"
#include "GameAPI.h"

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "SimpleMath.h"


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
        }
        
        void Update(float dt) override
        {
            Vector3 eye(0.0f, 0.7f, 1.5f);
            Vector3 at(0.0f, -0.1f, 0.0f);
            m_View = Matrix::CreateLookAt(eye, at, Vector3::UnitY);
            m_World = Matrix::CreateRotationY(dt * XM_PIDIV4);


            Keyboard::State kbState = GetKeyboard()->GetState();

            if (kbState.W) { std::cout << "Pressed W." << std::endl; }
            if (kbState.A) { std::cout << "Pressed A." << std::endl; }
            if (kbState.S) { std::cout << "Pressed S." << std::endl; }
            if (kbState.D) { std::cout << "Pressed D." << std::endl; }
        }

        void Render() override {}

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
            float aspectRatio = GetWidth() / GetHeight();

            float fovAngleY = 70.0f * XM_PI / 180.0f;

            // This is a simple example of change that can be made when the app is in
            // portrait or snapped view.
            if (aspectRatio < 1.0f)
            {
                fovAngleY *= 2.0f;
            }

            // This sample makes use of a right-handed coordinate system using row-major matrices.
            m_Projection = Matrix::CreatePerspectiveFieldOfView(
                fovAngleY,
                aspectRatio,
                0.01f,
                100.0f
            );
        }

    private:
        Matrix m_World;
        Matrix m_View;
        Matrix m_Projection;
};

EXPORT_GAME(Scratch);
