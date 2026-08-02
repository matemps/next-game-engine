cmake_minimum_required(VERSION 3.10.0)
project(next-game-engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# directx toolkit
add_subdirectory(extern/DirectXTK)

# graphics
add_subdirectory(graphics)

# launcher
add_subdirectory(launcher)

# game
add_subdirectory(scratch)

include(CTest)
enable_testing()

