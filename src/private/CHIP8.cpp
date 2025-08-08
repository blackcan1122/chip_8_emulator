#include "CHIP8.hpp"

CHIP8::CHIP8()
{
    m_CPU = new CPU();
    m_MEMORY = new Memory();

    
}

CHIP8::~CHIP8()
{
}
