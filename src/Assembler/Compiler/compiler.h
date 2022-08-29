#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <unordered_map>
#include "../Parser/instruction.h"

static std::unordered_map<std::string, int> OPCODES{
    {"dat", 0},
    {"mov", 1},

    {"add", 2},
    {"sub", 3},
    {"mul", 4},
    {"div", 5},
    {"mod", 6},

    {"jmp", 7},
    {"jmz", 8},
    {"jmn", 9},
    {"djn", 10},

    {"spl", 11},
    {"slt", 12},
    {"set", 13},
    {"sne", 14},

    {"nop", 15},
};

class Compiler
{
public:
    std::string compileProgram();
    std::string compileOperand(int);

    Compiler(std::vector<Instruction> instructionsParam)
    {
        instructions = instructionsParam;
    }

private:
    std::vector<Instruction> instructions;
};