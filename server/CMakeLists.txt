cmake_minimum_required(VERSION 3.18)
project(server C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(pos_sockets)

add_executable(server
        main.c
        pos_sockets/active_socket.c
        pos_sockets/active_socket.h
        pos_sockets/char_buffer.c
        pos_sockets/char_buffer.h
        pos_sockets/linked_list.h
        pos_sockets/passive_socket.c
        pos_sockets/passive_socket.h
        buffer.h
        hrac/hrac.h
        hrac/hrac.c
        hernaPlocha/hernaPlocha.c
        hernaPlocha/hernaPlocha.h)

target_link_libraries(server pthread)