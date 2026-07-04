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
        
        void Update() override {}

        void Shutdown() override
        {
            std::cout << "Game shutting down." << std::endl;
        }
};

EXPORT_GAME(Scratch);
