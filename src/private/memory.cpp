#include "memory.hpp"
#include <iostream>

void Memory::reset() {
    std::cout << "Memory reset." << std::endl;
}

uint8_t Memory::read(uint16_t address)
{
    std::cout << "Reading from memory address: " << address << std::endl;
}

void Memory::Write(uint16_t Address, uint8_t Value)
{
    if (Address >= 4096 || Address < InstructionSetStart || Address > InstructionSetEnd) {
        std::cerr << "Error: Attempt to write to invalid memory address: " << Address << std::endl;
        return;
    }
    if (Value < 0 || Value > 255) {
        std::cerr << "Error: Attempt to write invalid value: " << static_cast<int>(Value) << " to memory address: " << Address << std::endl;
        return;
    }
    m_Memory[Address] = Value;
    std::cout << "Writing value: " << static_cast<int>(Value) << " to memory address: " << Address << std::endl;
}
