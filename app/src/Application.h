#pragma once

#include "Game.h"
#include "Win32Window.h"


class Application
{
    public:
        // constructor
        Application(Game* game);

        // destructor
        ~Application();

    
    public:
        // member functions

        void Run();


    private:
        Game* m_Game = nullptr;
        Win32Window* m_Window = nullptr;
};