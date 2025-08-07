cmake_minimum_required(VERSION 3.15)

project(chip_8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(hidapi REQUIRED)
file(GLOB SRC_FILES src/*.cpp)
file(GLOB SRC_FILES_PRIVATE src/private/*.cpp)
file(GLOB SRC_FILES_PUBLIC src/public/*.cpp)


# Release Configuration
add_executable(chip_8 ${SRC_FILES})
target_link_libraries(chip_8 PRIVATE hidapi::hidapi)
target_compile_options(chip_8 PRIVATE -O3 -DNDEBUG)

# Debug Configuration with Symbols
add_executable(chip_8_debug ${SRC_FILES})
target_link_libraries(chip_8_debug PRIVATE hidapi::hidapi)
target_compile_definitions(chip_8_debug PRIVATE DEBUG=1)
target_compile_options(chip_8_debug PRIVATE -g -O0)
