cmake_minimum_required(VERSION 3.10)
project(Lab2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Lab2 main.c)

find_package(GSL REQUIRED)
target_link_libraries(Lab2 GSL::gsl GSL::gslcblas)