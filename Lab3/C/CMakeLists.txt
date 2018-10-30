cmake_minimum_required(VERSION 3.10)
project(C C)

set(CMAKE_C_STANDARD 99)

add_executable(C main.c)

find_package(GSL REQUIRED)
target_link_libraries(C GSL::gsl GSL::gslcblas)
