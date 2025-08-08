#include <gtest/gtest.h>
#include "../src/public/cpu.hpp"
#include <iostream>
#include <sstream>

class CPUTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu.reset();
    }
    
    void TearDown() override {
        // Clean up if needed
    }
    
    CPU cpu;
};

TEST_F(CPUTest, ConstructorTest) {
    // Capture cout to verify constructor message
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    CPU testCpu;
    
    std::cout.rdbuf(old);
    EXPECT_EQ(buffer.str(), "CPU initialized.\n");
}

TEST_F(CPUTest, ResetTest) {
    // Test that reset function can be called without error
    EXPECT_NO_THROW(cpu.reset());
}

TEST_F(CPUTest, ExecuteValidInstructionTest) {
    // Test executing valid instructions from the enum
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::OOE0));
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::OOEE));
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::NNN1));
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::FX65));
}

TEST_F(CPUTest, ClearDisplayInstructionTest) {
    // Capture cout to verify clear display message
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    cpu.executeInstruction(Instruction::OOE0);
    
    std::cout.rdbuf(old);
    EXPECT_EQ(buffer.str(), "Clearing display.\n");
}

TEST_F(CPUTest, ReturnFromSubroutineInstructionTest) {
    // Capture cout to verify return from subroutine message
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    cpu.executeInstruction(Instruction::OOEE);
    
    std::cout.rdbuf(old);
    EXPECT_EQ(buffer.str(), "Returning from subroutine.\n");
}

TEST_F(CPUTest, AllInstructionsExecuteWithoutCrash) {
    // Test that all instructions in the enum can be executed without crashing
    std::vector<Instruction> instructions = {
        Instruction::OOE0, Instruction::OOEE, Instruction::NNN1, Instruction::NNN2,
        Instruction::XNN3, Instruction::XNN4, Instruction::XY05, Instruction::XNN6,
        Instruction::XNN7, Instruction::XY08, Instruction::XY18, Instruction::XY28,
        Instruction::XY38, Instruction::XY48, Instruction::XY58, Instruction::XY68,
        Instruction::XY78, Instruction::XYE8, Instruction::XY09, Instruction::ANNN,
        Instruction::BNNN, Instruction::CXNN, Instruction::DXYN, Instruction::EX9E,
        Instruction::EXA1, Instruction::FX07, Instruction::FX0A, Instruction::FX15,
        Instruction::FX18, Instruction::FX1E, Instruction::FX29, Instruction::FX33,
        Instruction::FX55, Instruction::FX65
    };
    
    for (const auto& instruction : instructions) {
        EXPECT_NO_THROW(cpu.executeInstruction(instruction));
    }
}

TEST_F(CPUTest, InstructionTableBoundsTest) {
    // Test that all enum values have corresponding function pointers
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::OOE0));      // First instruction
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::FX65));      // Last instruction
}

TEST_F(CPUTest, MultipleInstructionExecutionTest) {
    // Test executing multiple instructions in sequence
    EXPECT_NO_THROW({
        cpu.executeInstruction(Instruction::OOE0);
        cpu.executeInstruction(Instruction::OOEE);
        cpu.executeInstruction(Instruction::NNN1);
        cpu.executeInstruction(Instruction::ANNN);
    });
}

TEST_F(CPUTest, CPUStateIntegrityTest) {
    // Test that CPU remains stable after multiple operations
    cpu.reset();
    
    for (int i = 0; i < 10; ++i) {
        cpu.executeInstruction(Instruction::OOE0);
        cpu.executeInstruction(Instruction::OOEE);
    }
    
    // CPU should still be functional
    EXPECT_NO_THROW(cpu.executeInstruction(Instruction::NNN1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}