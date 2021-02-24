cmake_minimum_required(VERSION 3.10)
project(cvd VERSION 1.0)

find_package(cpprestsdk REQUIRED)

configure_file(src/config.h.in head/config.h)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -lcrypto")

include_directories("${CMAKE_CURRENT_BINARY_DIR}/head")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} cpprestsdk::cpprest)
