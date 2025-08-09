#include "cpu.hpp"
#include <iostream>
#include "CHIP8.hpp"
#include "Display.hpp"
#include "Plattform.hpp"
#include <cstring>

// Constructor
CPU::CPU(CHIP8 *Outter)
{
    std::cout << "CPU initialized." << std::endl;
    m_PC = InstructionSetStart;
    m_SP = 0;
    m_Index = 0;
    m_DelayTimer = 0;
    m_SoundTimer = 0;
    m_State = CPUState::RUNNING;
    m_Outer = Outter;

    // Initialize main instruction table
    m_Table[0x0] = &CPU::Table0;
    m_Table[0x1] = &CPU::OP_1NNN;
    m_Table[0x2] = &CPU::OP_2NNN;
    m_Table[0x3] = &CPU::OP_3XNN;
    m_Table[0x4] = &CPU::OP_4XNN;
    m_Table[0x5] = &CPU::OP_5XY0;
    m_Table[0x6] = &CPU::OP_6XNN;
    m_Table[0x7] = &CPU::OP_7XNN;
    m_Table[0x8] = &CPU::Table8;
    m_Table[0x9] = &CPU::OP_9XY0;
    m_Table[0xA] = &CPU::OP_ANNN;
    m_Table[0xB] = &CPU::OP_BNNN;
    m_Table[0xC] = &CPU::OP_CXKK;
    m_Table[0xD] = &CPU::OP_DXYN;
    m_Table[0xE] = &CPU::TableE;
    m_Table[0xF] = &CPU::TableF;

    // Initialize Table0 (0x00XX opcodes)
    for (size_t i = 0; i < 0x10; i++)
    {
        m_Table0[i] = &CPU::OP_NULL;
    }
    m_Table0[0x0] = &CPU::OP_OOE0;
    m_Table0[0xE] = &CPU::OP_OOEE;

    // Initialize Table8 (0x8XYX opcodes)
    for (size_t i = 0; i < 0x10; i++)
    {
        m_Table8[i] = &CPU::OP_NULL;
    }
    m_Table8[0x0] = &CPU::OP_8XY0;
    m_Table8[0x1] = &CPU::OP_8XY1;
    m_Table8[0x2] = &CPU::OP_8XY2;
    m_Table8[0x3] = &CPU::OP_8XY3;
    m_Table8[0x4] = &CPU::OP_8XY4;
    m_Table8[0x5] = &CPU::OP_8XY5;
    m_Table8[0x6] = &CPU::OP_8XY6;
    m_Table8[0x7] = &CPU::OP_8XY7;
    m_Table8[0xE] = &CPU::OP_8XYE;

    // Initialize TableE (0xEXXX opcodes)
    for (size_t i = 0; i < 0x10; i++)
    {
        m_TableE[i] = &CPU::OP_NULL;
    }
    m_TableE[0x1] = &CPU::OP_EXA1;
    m_TableE[0xE] = &CPU::OP_EX9E;

    // Initialize TableF (0xFXXX opcodes)
    for (size_t i = 0; i < 0x70; i++)
    {
        m_TableF[i] = &CPU::OP_NULL;
    }
    m_TableF[0x07] = &CPU::OP_FX07;
    m_TableF[0x0A] = &CPU::OP_FX0A;
    m_TableF[0x15] = &CPU::OP_FX15;
    m_TableF[0x18] = &CPU::OP_FX18;
    m_TableF[0x1E] = &CPU::OP_FX1E;
    m_TableF[0x29] = &CPU::OP_FX29;
    m_TableF[0x33] = &CPU::OP_FX33;
    m_TableF[0x55] = &CPU::OP_FX55;
    m_TableF[0x65] = &CPU::OP_FX65;

    std::memset(m_Video, 0x0, sizeof(m_Video)); // Initialize video memory to black
}


// Reset function
void CPU::reset()
{
}

// Main CPU cycle - fetch, decode, execute
void CPU::cycle()
{

    m_OPcode = (m_Outer->getMemory()->read(m_PC) << 8u) | m_Outer->getMemory()->read(m_PC + 1u);

    // Increment PC before executing instruction
    m_PC += 2;

    ((*this).*(m_Table[(m_OPcode & 0xF000u) >> 12u]))();

    // Update timers
    if (m_DelayTimer > 0)
    {
        --m_DelayTimer;
    }

    if (m_SoundTimer > 0)
    {
        --m_SoundTimer;
    }
}

// Table dispatcher functions
inline void CPU::Table0()
{
    ((*this).*(m_Table0[m_OPcode & 0x000Fu]))();
}

inline void CPU::Table8()
{
    ((*this).*(m_Table8[m_OPcode & 0x000Fu]))();
}

inline void CPU::TableE()
{
    ((*this).*(m_TableE[m_OPcode & 0x000Fu]))();
}

inline void CPU::TableF()
{
    ((*this).*(m_TableF[m_OPcode & 0x00FFu]))();
}

// Invalid/unimplemented opcode handler
inline void CPU::OP_NULL()
{
    std::cout << "Invalid opcode: 0x" << std::hex << m_OPcode << std::endl;
    m_State = CPUState::ERROR;
}

inline void CPU::OP_OOE0()
{
    std::memset(m_Video, 0x00000000, sizeof(m_Video));
}

inline void CPU::OP_OOEE()
{
    --m_SP;
    m_PC = m_Stack[m_SP];
}

inline void CPU::OP_1NNN()
{
    uint16_t address = m_OPcode & 0x0FFFu;
    m_PC = address;
}

inline void CPU::OP_2NNN()
{
    Bit16 Adress = m_OPcode & 0x0FFFu;
    m_Stack[m_SP] = m_PC;
    ++m_SP;
    m_PC = Adress;
}

inline void CPU::OP_3XNN()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 NN = m_OPcode & 0x00FFu;
    if (m_Registers[Vx] == NN) 
    {
        m_PC += 2;
    }
}

inline void CPU::OP_4XNN()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 NN = m_OPcode & 0x00FFu;
    if (m_Registers[Vx] != NN) 
    {
        m_PC += 2;
    }
}

inline void CPU::OP_5XY0()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    if (m_Registers[Vx] == m_Registers[Vy]) 
    {
        m_PC += 2;
    }
}

inline void CPU::OP_6XNN()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 NN = m_OPcode & 0x00FFu;
    m_Registers[Vx] = NN;
}

inline void CPU::OP_7XNN()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 NN = m_OPcode & 0x00FFu;
    m_Registers[Vx] += NN;
}

inline void CPU::OP_8XY0()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    m_Registers[Vx] = m_Registers[Vy];
}

inline void CPU::OP_8XY1()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    m_Registers[Vx] |= m_Registers[Vy];
}

inline void CPU::OP_8XY2()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    m_Registers[Vx] &= m_Registers[Vy];
}

inline void CPU::OP_8XY3()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    m_Registers[Vx] ^= m_Registers[Vy];
}

inline void CPU::OP_8XY4()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    uint16_t sum = m_Registers[Vx] + m_Registers[Vy];
    if (sum > 0xFF) {
        m_Registers[0xF] = 1;
    } else {
        m_Registers[0xF] = 0;
    }

    m_Registers[Vx] = sum & 0xFF;
}

inline void CPU::OP_8XY5()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    if (m_Registers[Vx] > m_Registers[Vy]) {
        m_Registers[0xF] = 1; // Set VF to 1 if no borrow 
    } else {
        m_Registers[0xF] = 0; // Set VF to 0 if borrow
    }
    m_Registers[Vx] -= m_Registers[Vy];
}

inline void CPU::OP_8XY6()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_Registers[0xF] = m_Registers[Vx] & 0x01;
    m_Registers[Vx] >>= 1;
}

inline void CPU::OP_8XY7()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    if (m_Registers[Vy] > m_Registers[Vx]) {
        m_Registers[0xF] = 1;
    } else {
        m_Registers[0xF] = 0;
    }
    m_Registers[Vx] = m_Registers[Vy] - m_Registers[Vx];
}

inline void CPU::OP_8XYE()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_Registers[0xF] = (m_Registers[Vx] & 0x80) >> 7;
    m_Registers[Vx] <<= 1;
}

inline void CPU::OP_9XY0()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    if (m_Registers[Vx] != m_Registers[Vy]) {
        m_PC += 2;
    }
}

inline void CPU::OP_ANNN()
{
    Bit16 address = m_OPcode & 0x0FFFu;
    m_Index = address;
}

inline void CPU::OP_BNNN()
{
    Bit16 address = m_OPcode & 0x0FFFu;
    m_PC = address + m_Registers[0];
}

inline void CPU::OP_CXKK()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 NN = m_OPcode & 0x00FFu;
    m_Registers[Vx] = (rand() % 256) & NN;
}

inline void CPU::OP_DXYN()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Vy = (m_OPcode & 0x00F0u) >> 4u;
    Bit8 height = m_OPcode & 0x000Fu;

    Bit8 xPos = m_Registers[Vx] % VIDEO_WIDTH;
    Bit8 yPos = m_Registers[Vy] % VIDEO_HEIGHT;

    m_Registers[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row)
    {
        Bit8 SpriteByte = m_Outer->getMemory()->read(m_Index + row);

        for (unsigned int col = 0; col < 8; ++col)
        {
            Bit8 SpritePixel = SpriteByte & (0x80u >> col);
            
            uint32_t screenIndex = (yPos + row) * VIDEO_WIDTH + (xPos + col);
            
            if ((xPos + col) < VIDEO_WIDTH && (yPos + row) < VIDEO_HEIGHT)
            {
                if (SpritePixel) {
                    if (m_Video[screenIndex] != 0) {
                        m_Registers[0xF] = 1;
                    }
                    m_Video[screenIndex] ^= 0xFFFFFFFF;
                }
            }
        }
    }
}

inline void CPU::OP_EX9E()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Key = m_Registers[Vx];
    if (m_Keys[Key]) {
        m_PC += 2;
    }
}

inline void CPU::OP_EXA1()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    Bit8 Key = m_Registers[Vx];
    if (!m_Keys[Key]) {
        m_PC += 2;
    }
}

inline void CPU::OP_FX07()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_Registers[Vx] = m_DelayTimer;
}

inline void CPU::OP_FX0A()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    // Wait for a key press
    bool keyPressed = false;
    for (int i = 0; i < 16; ++i) 
    {
        if (m_Keys[i]) {
            m_Registers[Vx] = i;
            keyPressed = true;
            break;
        }
    }
    if (!keyPressed) {
        m_PC -= 2;
    }
}

inline void CPU::OP_FX15()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_DelayTimer = m_Registers[Vx];
}

inline void CPU::OP_FX18()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_SoundTimer = m_Registers[Vx];
}

inline void CPU::OP_FX1E()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_Index += m_Registers[Vx];
}

inline void CPU::OP_FX29()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    m_Index = CharacterStorageStart + (m_Registers[Vx] * 5);
}

inline void CPU::OP_FX33()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    uint8_t value = m_Registers[Vx];
    m_Outer->getMemory()->Write(m_Index, value / 100); // Hundreds
    m_Outer->getMemory()->Write(m_Index + 1, (value / 10) % 10); // Tens
    m_Outer->getMemory()->Write(m_Index + 2, value % 10); // Units
}

inline void CPU::OP_FX55()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    for (Bit8 i = 0; i <= Vx; ++i) {
        m_Outer->getMemory()->Write(m_Index + i, m_Registers[i]);
    }
}

inline void CPU::OP_FX65()
{
    Bit8 Vx = (m_OPcode & 0x0F00u) >> 8u;
    for (Bit8 i = 0; i <= Vx; ++i) {
        m_Registers[i] = m_Outer->getMemory()->read(m_Index + i);
    }
}
