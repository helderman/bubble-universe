cmake_minimum_required(VERSION 3.10.0)

project(BubbleUniverseD2D DESCRIPTION "Bubble Universe")

add_executable(bu bu.cpp)

target_link_libraries(bu PRIVATE d2d1)

target_link_options(bu PRIVATE "/SUBSYSTEM:WINDOWS")
