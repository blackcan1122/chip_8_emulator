#include <iostream>
#include "cpu.hpp"
#include "memory.hpp"

int main()
{
    CPU cpu;

    // Example instruction execution
    cpu.executeInstruction(Instruction::OOE0);
    cpu.executeInstruction(Instruction::OOEE);

    return 0;
}