#include <Windows.h>

#include <string>

#include "Application.h"
#include "Game.h"
#include "GameAPI.h"


int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    std::wstring dllPath = L"game.dll";

    HMODULE hGame = LoadLibraryW(dllPath.c_str());

    if (!hGame)
    {
        MessageBoxW(
            nullptr,
            (L"Failed to load " + dllPath).c_str(),
            L"Launcher Error",
            MB_ICONERROR
        );

        return -1;
    }

    auto createGame = reinterpret_cast<CreateGameFn>(
        GetProcAddress(hGame, "CreateGame")
    );

    if (!createGame)
    {
        MessageBoxW(
            nullptr,
            L"game.dll does not export CreateGame",
            L"Launcher Error",
            MB_ICONERROR
        );

        FreeLibrary(hGame);

        return -1;
    }

    {   // Application scope - app deleted before hGame is unloaded.

        Application app(createGame());
        app.Run();
    }

    FreeLibrary(hGame);

    return 0;
}
