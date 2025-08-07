#pragma once
#include <cstdint>
#include <vector>


class Memory {
    public:
        Memory() = default;
        ~Memory() = default;

        void reset();
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
private:
    uint8_t m_memory[4096]; // 4KB of memory
};