cmake_minimum_required(VERSION 3.15)

project(chip_8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

file(GLOB SRC_FILES src/*.cpp)
file(GLOB SRC_FILES_PRIVATE src/private/*.cpp)

set(ALL_SRC_FILES ${SRC_FILES} ${SRC_FILES_PRIVATE})

add_executable(chip_8 ${ALL_SRC_FILES})
target_include_directories(chip_8 PRIVATE src/public)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(chip_8 PRIVATE -g -O0 -Wall -Wextra)
    target_compile_definitions(chip_8 PRIVATE DEBUG=1)
else()
    target_compile_options(chip_8 PRIVATE -O3 -DNDEBUG)
endif()

# Option to build tests
option(BUILD_TESTS "Build the tests" OFF)

if(BUILD_TESTS)
    # Find GTest only when building tests
    find_package(GTest REQUIRED)
    
    # Create a library from your source files (excluding main.cpp)
    file(GLOB LIB_SRC_FILES src/private/*.cpp)
    add_library(chip_8_lib ${LIB_SRC_FILES})
    target_include_directories(chip_8_lib PUBLIC src/public)
    
    # Test files
    file(GLOB TEST_FILES tests/*.cpp)
    
    # Test executable
    add_executable(chip_8_tests ${TEST_FILES})
    target_link_libraries(chip_8_tests 
        chip_8_lib 
        ${GTEST_LIBRARIES} 
        pthread
    )
    target_include_directories(chip_8_tests PRIVATE src/public)
    
    # Enable testing
    enable_testing()
    add_test(NAME chip_8_unit_tests COMMAND chip_8_tests)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(chip_8_tests PRIVATE -g -O0 -Wall -Wextra)
        target_compile_definitions(chip_8_tests PRIVATE DEBUG=1)
    endif()
endif()