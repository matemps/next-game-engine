cmake_minimum_required(VERSION 3.10.0)
project(next-game-engine VERSION 0.1.0 LANGUAGES C CXX)

# Put all executables/DLLs in one shared output folder so the launcher can
# find game.dll next to itself at runtime without a post-build copy step.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# directx toolkit
add_subdirectory(extern/DirectXTK)

# box3d physics
add_subdirectory(extern/box3d)

# launcher
add_subdirectory(launcher)

# game
add_subdirectory(scratch)

include(CTest)
enable_testing()

