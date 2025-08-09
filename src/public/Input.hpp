#pragma once
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstdint>

// Global variables to store terminal state
static struct termios old_termios;
static bool terminal_initialized = false;

// Initialize terminal for non-blocking input
void InitTerminal() {
    if (!terminal_initialized) {
        tcgetattr(STDIN_FILENO, &old_termios);
        struct termios new_termios = old_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
        terminal_initialized = true;
    }
}

// Restore terminal to original state
void RestoreTerminal() {
    if (terminal_initialized) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
        terminal_initialized = false;
    }
}

// Check if input is available without blocking
bool kbhit() {
    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0;
}

bool ProcessInput(uint8_t* Keys) {
    static bool keys_held[16] = {false}; // Track key states
    bool Quit = false;
    
    // Initialize terminal on first call
    InitTerminal();
    
    // Process all available input
    while (kbhit()) {
        char ch = getchar();
        bool key_pressed = true;
        int chip8_key = -1;
        
        // Handle ESC key (quit)
        if (ch == 27) { // ESC
            Quit = true;
            continue;
        }
        
        // Map keyboard to CHIP-8 keys
        switch (ch) {
            case 'x': case 'X': chip8_key = 0x0; break;
            case '1': chip8_key = 0x1; break;
            case '2': chip8_key = 0x2; break;
            case '3': chip8_key = 0x3; break;
            case 'q': case 'Q': chip8_key = 0x4; break;
            case 'w': case 'W': chip8_key = 0x5; break;
            case 'e': case 'E': chip8_key = 0x6; break;
            case 'a': case 'A': chip8_key = 0x7; break;
            case 's': case 'S': chip8_key = 0x8; break;
            case 'd': case 'D': chip8_key = 0x9; break;
            case 'z': case 'Z': chip8_key = 0xA; break;
            case 'c': case 'C': chip8_key = 0xB; break;
            case '4': chip8_key = 0xC; break;
            case 'r': case 'R': chip8_key = 0xD; break;
            case 'f': case 'F': chip8_key = 0xE; break;
            case 'v': case 'V': chip8_key = 0xF; break;
        }
        
        if (chip8_key >= 0) {
            // Toggle key state (since we can't detect key release in terminal)
            keys_held[chip8_key] = !keys_held[chip8_key];
            Keys[chip8_key] = keys_held[chip8_key] ? 1 : 0;
            
            std::cout << "Key " << std::hex << chip8_key << (keys_held[chip8_key] ? " pressed" : " released") << std::endl;
        }
    }
    
    return Quit;
}