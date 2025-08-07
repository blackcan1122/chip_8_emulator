#pragma once
#include <cstdint>
#include <vector>
#include "memory.hpp"

class CPU {
public:
    CPU();
    ~CPU();

    void reset();
    void executeInstruction(uint16_t instruction);

private:
    uint8_t m_GPRegister[16]; // General Purpose Registers // V0 to VF (VF is a Flag register))
    uint16_t m_I; // Index Register
    std::vector<uint16_t> m_stack; // Stack
    uint8_t m_PC; // Program Counter
    uint8_t m_delayTimer; // Delay Timer
    uint8_t m_soundTimer; // Sound Timer
    Memory m_memory; // Memory instance

    inline void OP_OOE0(); // Clear the display
    inline void OP_OOEE(); // Return from a subroutine // Pop Address from Stack to PC
    inline void OP_1NNN(); // Jump to address NNN
    inline void OP_2NNN(); // Call subroutine at NNN // Push PC to Stack and SET PC = NNN
    inline void OP_3XNN(); // Skip next instruction if Vx == NN
    inline void OP_4XNN(); // Skip next instruction if Vx != NN
    inline void OP_5XY0(); // Skip next instruction if Vx == Vy
    inline void OP_6XNN(); // Set Vx = NN
    inline void OP_7XNN(); // Add NN to Vx
    inline void OP_8XY0(); // Set Vx = Vy
    inline void OP_8XY1(); // Set Vx = Vx OR Vy
    inline void OP_8XY2(); // Set Vx = Vx AND Vy
    inline void OP_8XY3(); // Set Vx = Vx XOR Vy
    inline void OP_8XY4(); // Add Vy to Vx, set VF = carry
    inline void OP_8XY5(); // Set VF = 1 if no Borrow
    inline void OP_8XY6(); // Shift Vx right by 1, set VF = LSB of Vx
    inline void OP_8XY7(); // Set Vx = Vy - Vx, set VF = 1 if no Borrow
    inline void OP_8XYE(); // Shift Vx left by 1, set VF = MSB of Vx
    inline void OP_9XY0(); // Skip next instruction if Vx != Vy
    inline void OP_ANNN(); // Set I = NNN
    inline void OP_BNNN(); // Jump to address NNN + V
    inline void OP_CXNN(); // Set Vx = random number AND NN
    inline void OP_DXYN(); // Draw N-byte sprite at (VX,VY)
    inline void OP_EX9E(); // Skip next instruction if key VX is pressed
    inline void OP_EXA1(); // Skip next instruction if key VX is not pressed
    inline void OP_FX07(); // Set Vx = delay timer
    inline void OP_FX0A(); // Wait for key press (blocking)
    inline void OP_FX15(); // Set delay timer = Vx
    inline void OP_FX18(); // Set sound timer = Vx
    inline void OP_FX1E(); // Add Vx to I
    inline void OP_FX29(); // Set I = location of sprite for digit Vx
    inline void OP_FX33(); // Store BCD representation of Vx in memory locations I, I+1, and I+2
    inline void OP_FX55(); // Store registers V0 through Vx in memory starting at location I
    inline void OP_FX65(); // Read registers V0 through Vx from memory starting at location I
};