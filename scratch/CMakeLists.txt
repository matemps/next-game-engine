add_library(scratch SHARED
    src/EASTLNew.cpp
    src/GameMain.cpp
)

target_include_directories(scratch PUBLIC
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/include/game
)

target_link_libraries(scratch PUBLIC EASTL)

target_compile_definitions(scratch PRIVATE GAME_EXPORTS)

set_target_properties(scratch PROPERTIES OUTPUT_NAME game)
