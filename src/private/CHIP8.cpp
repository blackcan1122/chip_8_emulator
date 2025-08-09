#include "CHIP8.hpp"
#include <fstream>
#include <iostream>

CHIP8::CHIP8()
{
    m_CPU = new CPU(this);
    m_MEMORY = new Memory();
    
    
    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        m_MEMORY->Write(CharacterStorageStart + i, Fontset[i]);
    }

    
}

CHIP8::~CHIP8()
{
}

void CHIP8::LoadRom(std::string RomPath)
{
    std::ifstream RomFile(RomPath, std::ios::binary | std::ios::ate);
    if (RomFile.is_open())
    {
        std::streampos RomSize = RomFile.tellg();
        char* RomBuffer = new char[RomSize];

        RomFile.seekg(0, std::ios::beg);
        RomFile.read(RomBuffer, RomSize);
        RomFile.close();

        for (long i = 0; i < RomSize; ++i)
        {
            m_MEMORY->Write(InstructionSetStart + i, static_cast<uint8_t>(RomBuffer[i]));
        }

        delete [] RomBuffer;
        std::cout << "ROM loaded successfully from: " << RomPath << std::endl;
    }

    else
    {
        std::cerr << "Failed to open ROM file: " << RomPath << std::endl;
    }
}
