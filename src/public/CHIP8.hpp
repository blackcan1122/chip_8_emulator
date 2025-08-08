#pragma once
#include "cpu.hpp"
#include "memory.hpp"

class CHIP8 {
public:
    CHIP8();
    ~CHIP8();

    CPU* getCPU() const { return m_CPU; }
    Memory* getMemory() const { return m_MEMORY; }

private:
    CPU *m_CPU;
    Memory *m_MEMORY;
};