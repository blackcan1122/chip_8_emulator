#pragma once
#include <cstdint>
#include <vector>
#include "memory.hpp"
#include "helpers.hpp"

#define V0 0
#define V1 1
#define V2 2
#define V3 3
#define V4 4
#define V5 5
#define V6 6
#define V7 7
#define V8 8
#define V9 9
#define VA 10
#define VB 11
#define VC 12
#define VD 13
#define VE 14
#define VF 15 // Flag

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32



enum class CPUState {
    RUNNING,
    HALTED,
    ERROR
};

enum class Instruction {
    OOE0, // Clear the display
    OOEE, // Return from a subroutine
    NNN1, // Jump to address NNN (1NNN)
    NNN2, // Call subroutine at NNN (2NNN)
    XNN3, // Skip next instruction if Vx == NN (3XNN)
    XNN4, // Skip next instruction if Vx != NN (4XNN)
    XY05, // Skip next instruction if Vx == Vy (5XY0)
    XNN6, // Set Vx = NN (6XNN)
    XNN7, // Add NN to Vx (7XNN)
    XY08, // Set Vx = Vy (8XY0)
    XY18, // Set Vx = Vx OR Vy (8XY1)
    XY28, // Set Vx = Vx AND Vy (8XY2)
    XY38, // Set Vx = Vx XOR Vy (8XY3)
    XY48, // Add Vy to Vx, set VF = carry (8XY4)
    XY58, // Subtract Vy from Vx, set VF = no borrow (8XY5)
    XY68, // Shift Vx right by 1, set VF = LSB (8XY6)
    XY78, // Set Vx = Vy - Vx, set VF = no borrow (8XY7)
    XYE8, // Shift Vx left by 1, set VF = MSB (8XYE)
    XY09, // Skip next instruction if Vx != Vy (9XY0)
    ANNN, // Set I = NNN
    BNNN, // Jump to address NNN + V0
    CXKK, // Set Vx = random number AND NN
    DXYN, // Draw N-byte sprite at (VX,VY)
    EX9E, // Skip next instruction if key VX is pressed
    EXA1, // Skip next instruction if key VX is not pressed
    FX07, // Set Vx = delay timer
    FX0A, // Wait for key press (blocking)
    FX15, // Set delay timer = Vx
    FX18, // Set sound timer = Vx
    FX1E, // Add Vx to I
    FX29, // Set I = location of sprite for digit Vx
    FX33, // Store BCD representation of Vx in memory locations I, I+1, and I+2
    FX55, // Store registers V0 through Vx in memory starting at location I
    FX65  // Read registers V0 through Vx from memory starting at location I
};

// Forward
class CHIP8;

class CPU {
public:
    CPU(CHIP8 *Outter);
    ~CPU() = default;

    void reset();
    void cycle(); // Main CPU cycle - fetch, decode, execute

private:
    friend class CHIP8;
    friend class Display;
    Bit8 m_Registers[16]; // General Purpose Registers // V0 to VF (VF is a Flag register))
    Bit16 m_Index; // Index Register
    Bit16 m_Stack[16]; // Stack
    Bit16 m_SP; // Stack Pointer
    Bit8 m_Keys[16]; // Keypad state (0-15)
    Bit16 m_PC; // Program Counter
    Bit8 m_DelayTimer; // Delay Timer
    Bit8 m_SoundTimer; // Sound Timer
    Bit16 m_OPcode; // Current opcode

    uint32_t m_Video[VIDEO_WIDTH * VIDEO_HEIGHT]; // Video memory (64x32 pixels)

    CPUState m_State; // Current state of the CPU

    CHIP8 *m_Outer;

    // Instruction implementations
    inline void OP_NULL(); // Invalid/unimplemented opcode
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
    inline void OP_CXKK(); // Set Vx = random number AND NN
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

    // Table dispatcher functions
    inline void Table0(); // Handle 0x0XXX opcodes
    inline void Table8(); // Handle 0x8XXX opcodes
    inline void TableE(); // Handle 0xEXXX opcodes
    inline void TableF(); // Handle 0xFXXX opcodes

    // Function pointer type for instruction handlers
    using InstructionHandler = void (CPU::*)();
    
    // Main instruction table (indexed by first nibble)
    InstructionHandler m_Table[0x10];
    
    // Sub-tables for opcodes that require further decoding
    InstructionHandler m_Table0[0x10];  // 0x00XX opcodes
    InstructionHandler m_Table8[0x10];  // 0x8XYX opcodes
    InstructionHandler m_TableE[0x10];  // 0xEXXX opcodes
    InstructionHandler m_TableF[0x70];  // 0xFXXX opcodes (up to 0x65)
};