#include <string>
#include <bitset>
#include <iostream>
#include <cstdlib>
#include "compiler.h"

std::string Compiler::compileProgram()
{
    std::string programString;
    for (Instruction instruction : instructions)
    {
        std::string opcode = std::bitset<4>(OPCODES[instruction.keyword]).to_string();
        std::string aMode = std::bitset<2>(instruction.A.addressingMode).to_string();
        std::string a = Compiler::compileOperand(instruction.A.value);
        std::string bMode = std::bitset<2>(instruction.B.addressingMode).to_string();
        std::string b = Compiler::compileOperand(instruction.B.value);

        programString += opcode + aMode + bMode + a + b;
    }
    return programString;
}

std::string Compiler::compileOperand(int operand)
{
    if (std::abs(operand) > 8000)
        operand = operand < 0 ? -(abs(operand) % 8000) : operand % 8000;

    if (operand < 0)
        operand = operand + 8000;

    return std::bitset<16>(operand).to_string();
}