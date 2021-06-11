cmake_minimum_required(VERSION 3.19)
project(mini_tar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Werror -Wshadow -pedantic")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(
        mini_tar
        main.cpp
        src/FileProcessor.cpp
        include/FileProcessor.h
        src/MiniTar.cpp
        include/MiniTar.h
        src/CLI.cpp
        include/CLI.h
        src/Creator.cpp
        include/Creator.h
        src/FileCreator.cpp
        include/FileCreator.h
        src/FileSerializer.cpp
        include/FileSerializer.h
        src/FileDeserializer.cpp
        include/FileDeserializer.h
        src/Extractor.cpp include/Extractor.h)
