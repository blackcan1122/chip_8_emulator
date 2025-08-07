#include "cpu.hpp"
#include <iostream>

constexpr CPU::InstructionHandler CPU::s_instructionTable[];

// Constructor
CPU::CPU()
{
    std::cout << "CPU initialized." << std::endl;
}


// Reset function
void CPU::reset()
{
}

// Execute instruction function
void CPU::executeInstruction(Instruction instruction)
{
    const auto instructionIndex = static_cast<size_t>(instruction);
    if (instructionIndex < sizeof(s_instructionTable) / sizeof(s_instructionTable[0])) {
        (this->*s_instructionTable[instructionIndex])();
    } else {
        // CPU PANIC: Invalid instruction
    }
}

// Instruction implementations
inline void CPU::OP_OOE0()
{
    std::cout << "Clearing display." << std::endl;
}

inline void CPU::OP_OOEE()
{
    std::cout << "Returning from subroutine." << std::endl;
}

inline void CPU::OP_1NNN()
{
}

inline void CPU::OP_2NNN()
{
}

inline void CPU::OP_3XNN()
{
}

inline void CPU::OP_4XNN()
{
}

inline void CPU::OP_5XY0()
{
}

inline void CPU::OP_6XNN()
{
}

inline void CPU::OP_7XNN()
{
}

inline void CPU::OP_8XY0()
{
}

inline void CPU::OP_8XY1()
{
}

inline void CPU::OP_8XY2()
{
}

inline void CPU::OP_8XY3()
{
}

inline void CPU::OP_8XY4()
{
}

inline void CPU::OP_8XY5()
{
}

inline void CPU::OP_8XY6()
{
}

inline void CPU::OP_8XY7()
{
}

inline void CPU::OP_8XYE()
{
}

inline void CPU::OP_9XY0()
{
}

inline void CPU::OP_ANNN()
{
}

inline void CPU::OP_BNNN()
{
}

inline void CPU::OP_CXNN()
{
}

inline void CPU::OP_DXYN()
{
}

inline void CPU::OP_EX9E()
{
}

inline void CPU::OP_EXA1()
{
}

inline void CPU::OP_FX07()
{
}

inline void CPU::OP_FX0A()
{
}

inline void CPU::OP_FX15()
{
}

inline void CPU::OP_FX18()
{
}

inline void CPU::OP_FX1E()
{
}

inline void CPU::OP_FX29()
{
}

inline void CPU::OP_FX33()
{
}

inline void CPU::OP_FX55()
{
}

inline void CPU::OP_FX65()
{
}
