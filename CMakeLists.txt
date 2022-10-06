cmake_minimum_required(VERSION 3.22)
project(Kalman_Filter C)

set(CMAKE_C_STANDARD 99)

add_executable(Kalman_Filter main.c kalman_filter/matrix.h kalman_filter/matrix.c kalman_filter/kalman.h kalman_filter/kalman.c)
