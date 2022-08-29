#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <algorithm>

#include "token.h"

Type Token::getType()
{
    return type;
}

std::string Token::getLiteral()
{
    return literal;
}

Type Token::validateKeyword()
{
    std::transform(literal.begin(), literal.end(), literal.begin(), ::tolower);
    if (isKeyword())
        return Type::KEYWORD;

    throw std::invalid_argument("Unexpected identifier in lexer: " + std::string(literal));
}

bool Token::isKeyword()
{
    if (std::find(KEYWORDS.begin(), KEYWORDS.end(), literal) != KEYWORDS.end())
    {
        return true;
    }

    return false;
}
