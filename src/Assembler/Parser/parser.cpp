#include <iostream>
#include <vector>
#include "parser.h"

void Parser::advance()
{
    ++position;
    token = tokens[position];
}

std::vector<Instruction> Parser::parse()
{
    std::vector<Instruction> program;
    while (token.type != Type::END)
    {
        Instruction instruction = parseInstruction();
        program.push_back(instruction);
    }

    return program;
}

Instruction Parser::parseInstruction()
{
    if (!token.isKeyword())
        throw std::invalid_argument("SyntaxError while parsing instruction: expected type keyword, got: " + token.literal);

    Instruction instruction(token.literal);
    advance();

    instruction.B = parseArgument();
    advance();

    if (token.type == Type::COMMA)
    {
        advance();
        instruction.A = instruction.B;
        instruction.B = parseArgument();
        advance();
    }

    return instruction;
}

Argument Parser::parseArgument()
{
    int addressingMode = 1;
    if (token.type != Type::NUMBER && token.type != Type::NEG)
    {
        if (token.type == Type::HASH)
            addressingMode = 0;
        else if (token.type == Type::AT)
            addressingMode = 2;
        else
            throw std::invalid_argument("SyntaxError, expected addressing mode, got: " + token.literal);

        advance();
    }

    bool negated = false;
    if (token.type == Type::NEG)
    {
        negated = true;
        advance();
    }
    else if (token.type != Type::NUMBER)
        throw std::invalid_argument("Expected type NUMBER after addressing mode, got: " + token.literal);

    return Argument(addressingMode, negated ? -std::stoi(token.literal) : std::stoi(token.literal));
}