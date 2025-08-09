#include <iostream>
#include "cpu.hpp"
#include "memory.hpp"
#include "CHIP8.hpp"
#include <thread>
#include <chrono>
#include "Plattform.hpp"
#include <filesystem>
#include <cstring>
#include "SDL3/SDL.h"

const int CPU_HZ = 500; // CHIP-8 CPU speed
const int DISPLAY_HZ = 60; // Display refresh rate
const int CYCLES_PER_FRAME = CPU_HZ / DISPLAY_HZ; // ~11-12 cycles per frame

typedef std::chrono::_V2::system_clock::time_point TimePoint;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM_PATH>" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) 
    {
        std::cout << "Usage: " << argv[0] << " <ROM_PATH>" << std::endl;
        std::cout << "Example: " << argv[0] << " mygame.ch8" << std::endl;
        return 0;
    }

    if (!std::filesystem::exists(argv[1])) 
    {
        std::cerr << "Error: ROM file '" << argv[1] << "' does not exist." << std::endl;
        return 1;
    }

    std::string RomPath = argv[1];
    Platform platform("CHIP-8 Emulator", 640, 320, 64, 32);

    CHIP8 chip8;
    chip8.LoadRom(RomPath);
    TimePoint LastTick;
    while (true) 
    {
        bool should_exit = platform.ProcessInput(chip8.getCPU()->getKeys());
        if (should_exit) {
            break;
        }
    
        // Run multiple CPU cycles per display frame
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            chip8.getCPU()->Cycle();
        }
    
        chip8.getCPU()->UpdateTimers();
        platform.Update(chip8.getCPU()->getVideoBuffer(), VIDEO_WIDTH * sizeof(uint32_t));

        SDL_Delay(16); // 60 FPS for display updates
    }

    return 0;
}