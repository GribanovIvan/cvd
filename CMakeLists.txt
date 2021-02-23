cmake_minimum_required(VERSION 3.10)
project(CVD VERSION 1.0)

configure_file(src/config.h.in src/config.h)

file(GLOB SOURCES src/*.cpp)

add_executable(cvd ${SOURCES})
