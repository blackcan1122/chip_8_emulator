#include <iostream>
#include "cpu.hpp"
#include "memory.hpp"
#include "CHIP8.hpp"

int main()
{
    CHIP8 chip8;
    CPU& cpu = *chip8.getCPU();
    Memory& memory = *chip8.getMemory();

    // Example instruction execution
    cpu.executeInstruction(Instruction::OOE0);
    cpu.executeInstruction(Instruction::OOEE);

    return 0;
}