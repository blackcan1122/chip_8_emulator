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
