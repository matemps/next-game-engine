add_library(scratch SHARED
    src/GameMain.cpp
)

target_include_directories(scratch PUBLIC
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/DirectXTK/Inc
)

target_link_libraries(scratch PUBLIC DirectXTK d3d11.lib dxgi.lib dxguid.lib)

target_compile_definitions(scratch PRIVATE GAME_EXPORTS)

set_target_properties(scratch PROPERTIES OUTPUT_NAME game)
