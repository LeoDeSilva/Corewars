#pragma once
#include <string>
#include <unordered_map>

class Argument
{
public:
    int addressingMode;
    int value;

    Argument(int addressingParam = 0, int valueParam = 0)
    {
        addressingMode = addressingParam;
        value = valueParam;
    }

    std::string toString()
    {
        return std::to_string(addressingMode) + " " + std::to_string(value);
    }
};

class Instruction
{
public:
    std::string keyword;
    Argument A;
    Argument B;

    Instruction(std::string keywordParam)
    {
        keyword = keywordParam;
    }

    std::string toString()
    {
        return keyword + +" A: " + A.toString() + " B: " + B.toString();
    };
};