cmake_minimum_required(VERSION 4.0)
project(CLion C)

set(CMAKE_C_STANDARD 23)

add_executable(CLion

        gbv.c
        util.c
        test2.c)
