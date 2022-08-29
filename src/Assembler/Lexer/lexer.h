#include <string>
#include <vector>
#include "../token.h"

class Lexer
{
public:
    std::vector<Token> tokenise();
    Token tokeniseChar();
    void eatWhitespace();

    bool isLetter();
    bool isDigit();
    std::string lexIdentifier();
    std::string lexNumber();

    Lexer(std::string fileString)
    {
        file = fileString;
        readPosition = 1;
        position = 0;
        curChar = file[position];
    }

private:
    std::string file;
    int position;
    int readPosition;
    char curChar;

    void advance();
    void peek();
};