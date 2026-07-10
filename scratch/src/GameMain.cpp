#include <string>
#include <iostream>

#include "Game.h"
#include "GameAPI.h"


class Scratch : public Game
{
    public:
        Scratch() : Game("Sample Game", 1280, 720) {}
    
    public:
        void Init() override
        {
            std::cout << "Game initialized." << std::endl;
        }
        
        void Update(float dt) override {}

        void Render() override {}

        void Shutdown() override
        {
            std::cout << "Game shutting down." << std::endl;
        }


        void OnActivated() override {}
        void OnDeactivated() override {}
        void OnSuspending() override {}
        void OnResuming() override {}
};

EXPORT_GAME(Scratch);
