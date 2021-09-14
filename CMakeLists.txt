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
        src/TarException.cpp
        include/TarException.h
        src/CLI.cpp
        include/CLI.h
        src/Creator.cpp
        include/Creator.h
        src/FileSerializer.cpp
        include/FileSerializer.h
        src/FileDeserializer.cpp
        include/FileDeserializer.h
        src/Extractor.cpp
        include/Extractor.h
        src/CLI.cpp
        src/Huffman.cpp
        src/HuffmanArchiver.cpp
        src/BitStream.cpp
        include/Huffman.h
        include/HuffmanArchiver.h
        include/BitStream.h
)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(
        test_huffman
        test/doctest.h
        test/TestMain.cpp
        src/CLI.cpp
        src/Huffman.cpp
        src/HuffmanArchiver.cpp
        src/BitStream.cpp
        include/CLI.h
        include/Huffman.h
        include/HuffmanArchiver.h
        include/BitStream.h
        test/TestBitStream.cpp
        test/TestHuffmanArchiver.cpp
        test/TestHuffman.cpp
)