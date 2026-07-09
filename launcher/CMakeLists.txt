add_executable(launcher WIN32
    ../common/EASTLNew.cpp
    src/main.cpp
    ../app/src/Application.cpp
    ../app/src/Win32Window.cpp
    ../input/Input.cpp
    ../timer/Timer.cpp
)

target_include_directories(launcher PRIVATE
    ../app/src
    ../public/game
)

target_link_libraries(launcher PRIVATE EASTL)

target_compile_definitions(launcher PRIVATE UNICODE _UNICODE)

add_dependencies(launcher scratch)