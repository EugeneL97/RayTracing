cmake_minimum_required(VERSION 3.30)
project(RayTracing C)

find_package(SDL2 REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(RayTracing main.c)

target_link_libraries(RayTracing SDL2 m)