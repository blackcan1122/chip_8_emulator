cmake_minimum_required(VERSION 3.15)

project(rk_driver)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(hidapi REQUIRED)
file(GLOB SRC_FILES src/*.cpp)


# Release Configuration
add_executable(rk_driver ${SRC_FILES})
target_link_libraries(rk_driver PRIVATE hidapi::hidapi)
target_compile_options(rk_driver PRIVATE -O3 -DNDEBUG)

# Debug Configuration with Symbols
add_executable(rk_driver_debug ${SRC_FILES})
target_link_libraries(rk_driver_debug PRIVATE hidapi::hidapi)
target_compile_definitions(rk_driver_debug PRIVATE DEBUG=1)
target_compile_options(rk_driver_debug PRIVATE -g -O0)
