add_library(scratch SHARED
    src/GameMain.cpp
)

target_include_directories(scratch PUBLIC
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/include/game
)

target_compile_definitions(scratch PRIVATE GAME_EXPORTS)

# Built as game.dll so the launcher can find it by a fixed name at runtime.
set_target_properties(scratch PROPERTIES OUTPUT_NAME game)
