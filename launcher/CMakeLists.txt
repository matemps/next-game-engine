add_executable(launcher WIN32
    src/main.cpp
    ../app/src/Application.cpp
    ../app/src/Win32Window.cpp
    ../input/Input.cpp
    ../graphics/DeviceResources.cpp
)

target_include_directories(launcher PRIVATE
    ../app/src
    ../public/game
    ../graphics
    ../timer
)

target_link_libraries(launcher PRIVATE d3d11.lib dxgi.lib dxguid.lib)

target_compile_definitions(launcher PRIVATE UNICODE _UNICODE)

add_dependencies(launcher scratch)