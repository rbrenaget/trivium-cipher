cmake_minimum_required(VERSION 3.25.1)

project(trivium VERSION 0.2)

file(GLOB SOURCES src/*.c)

include_directories(include)

add_library(trivium SHARED ${SOURCES})
target_include_directories(trivium PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add security compilation flags
target_compile_options(trivium PRIVATE -Wall -Wextra -Werror -fstack-protector-strong -D_FORTIFY_SOURCE=2 -O2 -Wformat -Wformat-security)
target_link_options(trivium PRIVATE -Wl,-z,relro,-z,now)

set_target_properties(trivium PROPERTIES VERSION ${PROJECT_VERSION})

# Set output directory for the library
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Install library and headers
install(TARGETS trivium DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Enable testing
enable_testing()

# Add test executable
add_executable(trivium_tests tests/test_trivium.c)
target_link_libraries(trivium_tests trivium)
target_include_directories(trivium_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add test to CTest
add_test(NAME trivium_unit_tests COMMAND trivium_tests)