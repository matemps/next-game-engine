#include "Game.h"


#if defined(GAME_EXPORTS)
    #define GAME_API __declspec(dllexport)
#else
    #define GAME_API
#endif


#define EXPORT_GAME(TGame) \
extern "C" GAME_API Game* CreateGame() \
{ \
    static_assert(std::is_base_of_v<Game, TGame>, "TGame must derive from Game"); \
    return new TGame(); \
}

// typedef (function signature) for GetGameInstance().
// Not used in custom game code.
using CreateGameFn = Game* (*)();