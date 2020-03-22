cmake_minimum_required(VERSION 3.0)

project(TestLibTrivium)

set(PROJECT_LINK_LIBS libtrivium.so)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

add_executable(libtriviumtest test.c)
target_link_libraries(libtriviumtest ${PROJECT_LINK_LIBS})