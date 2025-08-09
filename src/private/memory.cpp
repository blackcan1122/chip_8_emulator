#include "memory.hpp"
#include <iostream>

void Memory::reset() {
    std::cout << "Memory reset." << std::endl;
}

uint8_t Memory::read(uint16_t address)
{
    if (address >= 4096 || (address >= InstructionSetStart && address <= InstructionSetEnd)) {
        std::cerr << "Error: Attempt to read from invalid memory address: " << address << std::endl;
        return 0; // Return 0 for invalid addresses
    }
    uint8_t value = m_Memory[address];
    return value;
}

void Memory::Write(uint16_t Address, uint8_t Value)
{
    if (Address >= 4096 || (Address >= InstructionSetStart && Address <= InstructionSetEnd)) {
        std::cerr << "Error: Attempt to write to invalid memory address: " << Address << std::endl;
        return;
    }
    m_Memory[Address] = Value;
}
