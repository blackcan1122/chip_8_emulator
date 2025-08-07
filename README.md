# CHIP-8 Emulator

A modern C++17 implementation of the classic CHIP-8 virtual machine, designed for learning and retro gaming enthusiasts.

## About

CHIP-8 is an interpreted programming language developed in the mid-1970s for simple video games. This emulator faithfully recreates the CHIP-8 system, allowing you to run classic games like Pong, Tetris, and Space Invaders.

## Features

- **Complete CHIP-8 Instruction Set**: All 35 opcodes implemented
- **Memory Management**: 4KB RAM with proper addressing
- **CPU Emulation**: Registers, timers, and stack simulation
- **Modern C++**: Clean C++17 codebase with proper separation of concerns
- **Cross-Platform**: Built with CMake for easy compilation

## Getting Started

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15 or higher

### Building

```bash
# Clone the repository
git clone https://github.com/yourusername/CHIP_8_EMU.git
cd CHIP_8_EMU

# Create build directory
mkdir build && cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run the emulator
./chip_8
```


## 🏗️ Architecture

```
src/
├── public/          # Header files
│   ├── cpu.hpp      # CPU emulation and instruction set
│   └── memory.hpp   # Memory management (4KB RAM)
├── private/         # Implementation files
│   ├── cpu.cpp      # CPU instruction implementations
│   └── memory.cpp   # Memory read/write operations
└── main.cpp         # Application entry point
```

## 🎮 CHIP-8 Specifications

- **Memory**: 4KB (4096 bytes)
- **Registers**: 16 general-purpose 8-bit registers (V0-VF)
- **Index Register**: 16-bit register for memory addressing
- **Stack**: 16 levels for subroutine calls
- **Timers**: 60Hz delay and sound timers
- **Display**: 64x32 monochrome pixels


## TODO

- [ ] Display/Graphics system
- [ ] Input handling (16-key keypad)
- [ ] Sound system
- [ ] ROM loading functionality
- [ ] Debugger interface
- [ ] Game compatibility testing

---

*Built with ❤️ for retro computing enthusiasts*