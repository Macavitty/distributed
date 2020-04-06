cmake_minimum_required(VERSION 3.15)
project(distributed_n C)

set(CMAKE_C_STANDARD 99)

add_executable(distributed_n main.c ipc.c logger.h logger.c)