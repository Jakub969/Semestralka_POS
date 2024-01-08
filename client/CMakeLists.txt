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
## ak sa pouziva Visual Studio toolchain
#[[
    find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(client PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(client "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_link_libraries(client wsock32 ws2_32 Threads::Threads)
]]
target_link_libraries(client wsock32 ws2_32 pthread)
