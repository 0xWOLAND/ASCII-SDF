cmake_minimum_required(VERSION 3.21)
project(Cube)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Cube
        conio.h
        main.cpp
        raycast.cpp
        sdf.cpp
        sphere_cut.cpp
        vec2.h
        vec3.h
        winuser_test.cpp)

add_executable(sdf sdf.cpp)