cmake_minimum_required(VERSION 3.5)

project(cvd++ LANGUAGES CXX)

add_executable(cvd++ main.cpp)
target_link_libraries(cvd++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
