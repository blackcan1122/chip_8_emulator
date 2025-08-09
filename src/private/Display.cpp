#include "Display.hpp"
#include "cpu.hpp"
#include <iostream>
#include "CHIP8.hpp"

Display::Display(CHIP8 *Outer) : m_Outer(Outer)
{
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; ++i) 
    {
        m_Outer->getCPU()->m_Video[i] = 0xFFFFFFFF; // Initialize video memory to white
    }
    std::cout << "Display initialized." << std::endl;
}

void Display::Draw()
{
    for (int y = 0; y < VIDEO_HEIGHT; ++y) 
    {
        for (int x = 0; x < VIDEO_WIDTH; ++x) 
        {
            uint32_t pixel = m_Outer->getCPU()->m_Video[y * VIDEO_WIDTH + x];

            std::cout << (pixel == 0xFFFFFFFF ? " " : "█");
        }
        std::cout << std::endl;
    }
}

void Display::ClearBuffer()
{
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; ++i) 
    {
        m_Outer->getCPU()->m_Video[i] = 0xFFFFFFFF; // Reset video memory to white
    }
}
