#pragma once
#include <cstdint>
#include <vector>
#include "helpers.hpp"


#define CharacterStorageStart 0x050 // Start of character storage in memory
#define CharacterStorageEnd   0x0A0 // End of character storage in memory
#define InstructionSetStart 0x200 // Start of instruction set in memory
#define InstructionSetEnd   0xFFF // End of instruction set in memory

class Memory {
    public:
        Memory() = default;
        ~Memory() = default;

        void reset();
        uint8_t read(uint16_t address);
        void Write(uint16_t address, uint8_t value);
private:
    Bit8 m_Memory[4096]; // 4KB of memory
};