cmake_minimum_required (VERSION 3.24)                                                     
project(tcpTransfer C)

find_package (Threads)

add_executable(server.o
        src/serverMain.c
        src/stuff.c
)

add_executable(client.o
        src/clientMain.c 
        src/stuff.c
)

SET(WARN_FLAGS "-Wall -Wpedantic -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${WARN_FLAGS} ${ASAN_OPTIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

target_link_libraries (server.o ${CMAKE_THREAD_LIBS_INIT})
