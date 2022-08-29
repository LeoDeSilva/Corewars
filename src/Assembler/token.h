#pragma once
#include <string>
#include <vector>

const std::vector<std::string> KEYWORDS{
    "mov",
    "dat",
    "add",
    "sub",
    "mul",
    "div",
    "mod",
    "jmp",
    "jmz",
    "jmn",
    "djn",
    "spl",
    "slt",
    "seq",
    "sne",
    "nop",
};

enum class Type
{
    KEYWORD,
    NUMBER,
    HASH,
    AT,
    NEG,
    COMMA,
    NIL,
    NEWLINE,
    END,
};

class Token
{
public:
    Type type;
    std::string literal;

    Type getType();
    Type validateKeyword();
    bool isKeyword();
    std::string getLiteral();

    Token(Type typeParam, std::string literalParam)
    {
        type = typeParam;
        literal = literalParam;
    }
};