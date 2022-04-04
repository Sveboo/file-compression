cmake_minimum_required(VERSION 3.20.2)
project(file_compression C)

set(CMAKE_C_STANDARD 99)

add_executable(file_compression main.c Huffman.c dict.c)
