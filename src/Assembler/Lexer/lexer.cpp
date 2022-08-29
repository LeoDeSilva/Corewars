#include <stdexcept>
#include <iostream>
#include <format>
#include <vector>
#include "lexer.h"

void Lexer::peek()
{
}

void Lexer::advance()
{
    if (readPosition > file.length())
        curChar = 0;
    else
        curChar = file[readPosition];

    position = readPosition;
    ++readPosition;
}

std::vector<Token> Lexer::tokenise()
{
    std::vector<Token> Tokens;
    while (curChar != 0)
    {
        Token token = tokeniseChar();
        Tokens.push_back(token);
    }

    Tokens.push_back(Token(Type::END, ""));
    return Tokens;
}

Token Lexer::tokeniseChar()
{
    Token tok(Type::END, "");
    // TODO: if issues, tokenise NEW_LINE for use in parser
    eatWhitespace();
    switch (curChar)
    {
    case '#':
        tok.type = Type::HASH;
        tok.literal = curChar;
        break;

    case '@':
        tok.type = Type::AT;
        tok.literal = curChar;
        break;

    case ',':
        tok.type = Type::COMMA;
        tok.literal = curChar;
        break;

    case '-':
        tok.type = Type::NEG;
        tok.literal = curChar;
        break;

    case '\n':
        tok.type = Type::NEWLINE;
        tok.literal = curChar;
        break;

    default:
        if (isLetter())
        {
            tok.literal = lexIdentifier();
            tok.type = tok.validateKeyword();
            return tok;
        }
        else if (isDigit())
        {
            tok.literal = lexNumber();
            tok.type = Type::NUMBER;
            return tok;
        }

        throw std::invalid_argument("Unexpected character in lexer: " + std::string(1, curChar));
    }

    advance();
    return tok;
}

std::string Lexer::lexNumber()
{
    std::string number;
    while (isDigit())
    {
        number += curChar;
        advance();
    }

    return number;
}

std::string Lexer::lexIdentifier()
{
    std::string identifier;
    while (isLetter())
    {
        identifier += curChar;
        advance();
    }

    return identifier;
}

bool Lexer::isDigit()
{
    return '0' <= curChar && curChar <= '9';
}

bool Lexer::isLetter()
{
    return 'a' <= curChar && curChar <= 'z' || 'A' <= curChar && curChar <= 'Z';
}

void Lexer::eatWhitespace()
{
    while (isspace(curChar)) // && curChar != '\n')
        advance();
}