cmake_minimum_required(VERSION 3.10.0)
project(next-game-engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# directx toolkit
add_subdirectory(extern/DirectXTK)

# box3d physics
add_subdirectory(extern/box3d)

# graphics
add_subdirectory(graphics)

# launcher
add_subdirectory(launcher)

# game
add_subdirectory(scratch)

include(CTest)
enable_testing()

