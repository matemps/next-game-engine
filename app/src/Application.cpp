#include <string>

#include "Application.h"
#include "Win32Window.h"
#include "Game.h"
#include "../../input/Input.h"
#include "../../timer/Timer.h"


// constructor
Application::Application(Game* game) : m_Game(game)
{
    std::string title = game->GetTitle();
    int width = game->GetWidth();
    int height = game->GetHeight();

    std::wstring wideTitle(title.begin(), title.end());
    m_Window = new Win32Window(wideTitle.c_str(), width, height);

    Input::Initialize();    // init input

    game->Init();   // init game
}


void Application::Run()
{
    Timer timer;
    
    while (m_Window->ProcessMessages())
    {
        timer.Tick();
        float dt = timer.GetDeltaTime();

        if (m_Game)
        {
            m_Game->Update(dt);
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