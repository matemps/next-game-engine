#include <string>

#include "Application.h"
#include "Win32Window.h"
#include "Game.h"


// constructor
Application::Application(Game* game) : m_Game(game)
{
    std::string title = game->GetTitle();
    int width = game->GetWidth();
    int height = game->GetHeight();

    std::wstring wideTitle(title.begin(), title.end());
    m_Window = new Win32Window(wideTitle.c_str(), width, height);

    game->Init();   // init game
}


void Application::Run()
{
    while (m_Window->ProcessMessages())
    {
        if (m_Game)
        {
            m_Game->Update();
        }
    }
}


// destructor
Application::~Application()
{
    if (m_Game)
    {
        m_Game->Shutdown();

        delete m_Game;
    }

    delete m_Window;
}