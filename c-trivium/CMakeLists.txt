cmake_minimum_required(VERSION 3.0)

project(trivium VERSION 0.1)

include_directories(inc)

file(GLOB SOURCES src/*.c)

add_executable(testTrivium ${SOURCES})
