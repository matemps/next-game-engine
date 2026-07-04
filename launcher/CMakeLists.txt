add_executable(launcher WIN32
    src/main.cpp
    ../app/src/Application.cpp
    ../app/src/Win32Window.cpp
)

target_include_directories(launcher PRIVATE
    ../app/src
    ../public/game
)

target_compile_definitions(launcher PRIVATE UNICODE _UNICODE)

# launcher builds the game DLL so it's present next to the exe.
add_dependencies(launcher scratch)