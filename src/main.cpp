#include <iostream>
#include "cpu.hpp"
#include "memory.hpp"
#include "CHIP8.hpp"
#include <thread>
#include <chrono>

int main()
{
    CHIP8 chip8;
    chip8.LoadRom("/home/blackcan/Documents/Hax0rStuff/CHIP_8_EMU/roms/Pong (1 player).ch8");
    while (true) {
        chip8.getCPU()->cycle(); // Execute one CPU cycle
        chip8.getDisplay()->Draw(); // Draw the display
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Sleep for 16ms to simulate ~60Hz refresh rate
    }

    return 0;
}