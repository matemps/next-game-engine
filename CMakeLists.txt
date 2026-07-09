cmake_minimum_required(VERSION 3.10.0)
project(next-game-engine VERSION 0.1.0 LANGUAGES C CXX)

# Put all executables/DLLs in one shared output folder so the launcher can
# find game.dll next to itself at runtime without a post-build copy step.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# eastl
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
add_subdirectory(extern/EASTL)

# launcher
add_subdirectory(launcher)

# game
add_subdirectory(scratch)

include(CTest)
enable_testing()

