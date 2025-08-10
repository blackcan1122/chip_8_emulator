cmake_minimum_required(VERSION 3.15)

project(chip_8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# Build SDL3 from source
set(SDL_SHARED ON CACHE BOOL "Build a shared version of the library")
set(SDL_STATIC OFF CACHE BOOL "Build a static version of the library")
set(SDL_TEST OFF CACHE BOOL "Build the SDL test programs")
set(SDL_EXAMPLES OFF CACHE BOOL "Build the SDL example programs")

# Disable some SDL3 features that might cause cross-compilation issues
if(CMAKE_CROSSCOMPILING)
    set(SDL_RENDER_D3D OFF CACHE BOOL "Enable the Direct3D render driver")
    set(SDL_VULKAN OFF CACHE BOOL "Enable Vulkan support")
    set(SDL_METAL OFF CACHE BOOL "Enable Metal support")
    set(SDL_OPENGLES OFF CACHE BOOL "Enable OpenGL ES support")
    set(SDL_OPENGL ON CACHE BOOL "Enable OpenGL support")
    set(SDL_AUDIO ON CACHE BOOL "Enable audio subsystem")
    set(SDL_VIDEO ON CACHE BOOL "Enable video subsystem")
    set(SDL_RENDER ON CACHE BOOL "Enable render subsystem")
    set(SDL_EVENTS ON CACHE BOOL "Enable events subsystem")
    set(SDL_JOYSTICK ON CACHE BOOL "Enable joystick subsystem")
    set(SDL_HAPTIC ON CACHE BOOL "Enable haptic subsystem")
    set(SDL_HIDAPI ON CACHE BOOL "Enable HIDAPI subsystem")
    set(SDL_POWER ON CACHE BOOL "Enable power subsystem")
    set(SDL_THREADS ON CACHE BOOL "Enable threading subsystem")
    set(SDL_TIMERS ON CACHE BOOL "Enable timers subsystem")
    set(SDL_FILE ON CACHE BOOL "Enable file subsystem")
    set(SDL_LOADSO ON CACHE BOOL "Enable dynamic library loading")
    set(SDL_CPUINFO ON CACHE BOOL "Enable CPU info")
    set(SDL_FILESYSTEM ON CACHE BOOL "Enable filesystem subsystem")
    set(SDL_SENSOR ON CACHE BOOL "Enable sensor subsystem")
    set(SDL_LOCALE ON CACHE BOOL "Enable locale subsystem")
    set(SDL_MISC ON CACHE BOOL "Enable miscellaneous subsystem")
endif()

# Add SDL3 subdirectory
add_subdirectory(thirdparty/SDL)

file(GLOB SRC_FILES src/*.cpp)
file(GLOB SRC_FILES_PRIVATE src/private/*.cpp)

set(ALL_SRC_FILES ${SRC_FILES} ${SRC_FILES_PRIVATE})

add_executable(chip_8 ${ALL_SRC_FILES})
target_include_directories(chip_8 PRIVATE src/public)

# Link with SDL3 (now built from source)
target_link_libraries(chip_8 
    SDL3::SDL3
)

# Platform-specific linking for Windows MinGW
if(MINGW)
    # Static linking for Windows to avoid runtime dependency issues
    target_link_libraries(chip_8 
        mingw32
        winmm
        imm32
        ole32
        oleaut32
        uuid
        advapi32
        shell32
        version
        setupapi
        gdi32
        user32
        opengl32
        -static-libgcc
        -static-libstdc++
    )
    
    # Additional linker flags for static linking
    set_target_properties(chip_8 PROPERTIES
        LINK_FLAGS "-static -static-libgcc -static-libstdc++"
    )
endif()

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