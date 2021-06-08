cmake_minimum_required(VERSION 3.19)
project(mini_tar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wshadow -pedantic")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(
        mini_tar
        main.cpp
        src/File.cpp
        include/File.h
        src/MiniTar.cpp
        include/MiniTar.h
        src/CLI.cpp
        include/CLI.h
        src/Creator.cpp include/Creator.h)
