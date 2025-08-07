#pragma once
#include <cstdint>
#include <vector>
#include "memory.hpp"

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
    CXNN, // Set Vx = random number AND NN
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

class CPU {
public:
    CPU();
    ~CPU() = default;

    void reset();
    void executeInstruction(Instruction instruction);

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

    // Function pointer type for instruction handlers
    using InstructionHandler = void (CPU::*)();
    
    // Array of function pointers indexed by instruction enum
    static constexpr InstructionHandler s_instructionTable[] = {
        &CPU::OP_OOE0,  // Instruction::OOE0
        &CPU::OP_OOEE,  // Instruction::OOEE
        &CPU::OP_1NNN,  // Instruction::NNN1
        &CPU::OP_2NNN,  // Instruction::NNN2
        &CPU::OP_3XNN,  // Instruction::XNN3
        &CPU::OP_4XNN,  // Instruction::XNN4
        &CPU::OP_5XY0,  // Instruction::XY05
        &CPU::OP_6XNN,  // Instruction::XNN6
        &CPU::OP_7XNN,  // Instruction::XNN7
        &CPU::OP_8XY0,  // Instruction::XY08
        &CPU::OP_8XY1,  // Instruction::XY18
        &CPU::OP_8XY2,  // Instruction::XY28
        &CPU::OP_8XY3,  // Instruction::XY38
        &CPU::OP_8XY4,  // Instruction::XY48
        &CPU::OP_8XY5,  // Instruction::XY58
        &CPU::OP_8XY6,  // Instruction::XY68
        &CPU::OP_8XY7,  // Instruction::XY78
        &CPU::OP_8XYE,  // Instruction::XYE8
        &CPU::OP_9XY0,  // Instruction::XY09
        &CPU::OP_ANNN,  // Instruction::ANNN
        &CPU::OP_BNNN,  // Instruction::BNNN
        &CPU::OP_CXNN,  // Instruction::CXNN
        &CPU::OP_DXYN,  // Instruction::DXYN
        &CPU::OP_EX9E,  // Instruction::EX9E
        &CPU::OP_EXA1,  // Instruction::EXA1
        &CPU::OP_FX07,  // Instruction::FX07
        &CPU::OP_FX0A,  // Instruction::FX0A
        &CPU::OP_FX15,  // Instruction::FX15
        &CPU::OP_FX18,  // Instruction::FX18
        &CPU::OP_FX1E,  // Instruction::FX1E
        &CPU::OP_FX29,  // Instruction::FX29
        &CPU::OP_FX33,  // Instruction::FX33
        &CPU::OP_FX55,  // Instruction::FX55
        &CPU::OP_FX65   // Instruction::FX65
    };
};