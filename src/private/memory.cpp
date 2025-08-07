#include "memory.hpp"
#include <iostream>

void Memory::reset() {
    std::cout << "Memory reset." << std::endl;
}

uint8_t Memory::read(uint16_t address)
{
    std::cout << "Reading from memory address: " << address << std::endl;
}

void Memory::write(uint16_t address, uint8_t value)
{
    std::cout << "Writing to memory address: " << address << " Value: " << static_cast<int>(value) << std::endl;
}
