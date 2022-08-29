#include <vector>
#include <string>
#include "../token.h"
#include "instruction.h"

class Parser
{
public:
    std::vector<Instruction> parse();
    Parser(std::vector<Token> tokenParams) : token(Type::NIL, "")
    {
        tokens = tokenParams;
        position = 0;

        token = tokens[position];
    }

private:
    int position;
    std::vector<Token> tokens;
    Token token;
    Instruction parseInstruction();
    Argument parseArgument();

    void advance();
};