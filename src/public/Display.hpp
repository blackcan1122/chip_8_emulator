#pragma once

// Forward
class CHIP8;

class Display {
public:
    Display(CHIP8 *Outer);
    ~Display() = default;

    void Draw();
    void ClearBuffer();

private:
    CHIP8 *m_Outer;
};
