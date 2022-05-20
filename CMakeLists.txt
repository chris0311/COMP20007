cmake_minimum_required(VERSION 3.21)
project(ASS2 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(ASS2
        include/crypto.h
        include/iohelpers.h
        include/permutation.h
        include/sponge.h
        src/crypto.c
        src/decr.c
        src/encr.c
        src/hash.c
        src/iohelpers.c
        src/mac.c
        src/permutation.c
        src/sponge.c)
