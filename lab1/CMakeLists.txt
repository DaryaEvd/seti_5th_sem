cmake_minimum_required (VERSION 3.24) 
project(copyFinder)

find_package (Threads)

add_executable(finder
        src/main.c
        src/copyList.c
        src/listener.c
        src/sender.c
        src/addrStuff.c
)

SET(WARN_FLAGS "-Wall -Wpedantic -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${WARN_FLAGS} ${ASAN_OPTIONS}") 
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

target_link_libraries (finder ${CMAKE_THREAD_LIBS_INIT})
