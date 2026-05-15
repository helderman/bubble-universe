cmake_minimum_required(VERSION 3.21)

project(BubbleUniverseSDL DESCRIPTION "Bubble Universe")

add_executable(bu bu.c)

# https://wiki.libsdl.org/SDL2/README-cmake
# On Windows: download SDL2-devel-2.*.*-VC.zip, unzip,
# set environment variable SDL2_ROOT = full path of folder SDL2-2.*.*
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

target_link_libraries(bu PRIVATE SDL2::SDL2)

# Copy SDL2.dll into the build folder (for VSCode launch/debug)
# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
add_custom_command(TARGET bu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:bu> $<TARGET_RUNTIME_DLLS:bu>
    COMMAND_EXPAND_LISTS
)

# Copy SDL2.dll into the installation folder
# https://stackoverflow.com/a/75065206/2485966
install(TARGETS bu)
install(FILES $<TARGET_RUNTIME_DLLS:bu> TYPE BIN)
