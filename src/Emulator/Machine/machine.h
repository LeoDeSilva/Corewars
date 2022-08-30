#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

class VirtualMachine
{
public:
    std::vector<int> queueA;
    std::vector<int> queueB;

    VirtualMachine() : window(sf::VideoMode(1440, 1440), "DISPLAY")
    {
        for (int i = 0; i < 40000; ++i)
            memory[i] = 0;
    }

    void Load(std::vector<unsigned char> program, int addr);
    void Dump(int, int);

    void startEmulation();

private:
    unsigned char memory[40000];
    unsigned short ADR;
    unsigned short BDR;
    unsigned short AAR;
    unsigned short BAR;
    unsigned char A_MODE;
    unsigned char B_MODE;
    sf::RenderWindow window;
    bool playerOne;

    void display();
    void MOV();
    void OP(std::string);
    void JMP();
    void JMZ();
    void JMN();
    void DJN();
    void SPL();

    void setBlock(std::vector<int>, int);
    void setData(unsigned short, int);
    void setOperand(unsigned short, int);

    std::vector<int> REF_A;
    std::vector<int> REF_B;

    bool emulateCycle();
    void readOperand(int);
    std::vector<int> fetchOperand(unsigned char, unsigned short, int);
    int PC;

    unsigned short read(std::vector<int> ref)
    {
        return (memory[ref[0]] << 8) | memory[ref[0] + 1];
    }

    unsigned char fetch(int pointer)
    {
        return memory[pointer % 40000];
    }

    int mod(int value)
    {
        return value % 40000;
    }
};