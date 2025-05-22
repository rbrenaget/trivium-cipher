cmake_minimum_required(VERSION 3.25.1)

project(TestLibTrivium)

# Create test executable FIRST
add_executable(libtriviumtest test.c)

# Include directories (after creating the target)
target_include_directories(libtriviumtest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Find the trivium library
find_library(TRIVIUM_LIB
    NAMES trivium libtrivium
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/
    REQUIRED
)

# Link libraries
target_link_libraries(libtriviumtest PRIVATE ${TRIVIUM_LIB})

# Set C standard
set_target_properties(libtriviumtest PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

# Enable testing
enable_testing()
add_test(NAME trivium_test COMMAND libtriviumtest)S