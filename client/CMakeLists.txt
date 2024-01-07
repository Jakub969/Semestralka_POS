cmake_minimum_required(VERSION 3.18)
project(client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(client
        main.cpp
        my_socket.cpp
        my_socket.h
        hrac/Hrac.cpp
        hernaPlocha/HernaPlocha.cpp)

target_link_libraries(client pthread wsock32 ws2_32)