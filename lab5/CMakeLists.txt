cmake_minimum_required(VERSION 3.24)
project(Proxy-server C)
 
add_executable(proxy.o
        src/main.c
        src/event.c
        src/handlerConnections.c
        src/common.c
        src/handlerUDP.c
        src/handlerTCP.c
        )

SET(WARN_FLAGS "-Wall -Wpedantic -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_FLAGS} ${ASAN_OPTIONS}") 
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
