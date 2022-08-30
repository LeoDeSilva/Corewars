#include <iostream>
#include <bitset>
#include "machine.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

unsigned short operation(std::string op, int a, int b)
{
    if (op == "ADD")
        return (a + b) % 40000;
    if (op == "SUB")
        return (a - b) % 40000;
    if (op == "MUL")
        return (a * b) % 40000;
    if (op == "DIV")
        return (a / b) % 40000;
    if (op == "MOD")
        return (a % b) % 40000;

    return 0;
}
void VirtualMachine::Dump(int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        if (i % 5 == 0 && i > 0)
            std::cout << std::bitset<8>(memory[i]).to_string() << ", ";
        else
            std::cout << std::bitset<8>(memory[i]).to_string() << " ";
    }

    std::cout << "\n";
}

void VirtualMachine::Load(std::vector<unsigned char> program, int addr)
{
    for (int i = 0; i < program.size(); ++i)
    {
        memory[i + addr] = program[i];
    }
}

void VirtualMachine::startEmulation()
{
    bool playerOne = true;
    int oneInit = queueA[0];
    int twoInit = queueB[0];
    // for (;;)
    int counter = 0;
    int max = 48000;
    while (queueA.size() > 0 && queueB.size() > 0 && window.isOpen())
    // for (int i = 0; i < 4; ++i)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        display();
        window.display();

        if (counter > max)
            break;

        if (playerOne)
        {
            // std::cout << "PLAYER 1:\n";
            PC = queueA[0];
            queueA.erase(queueA.begin());
            // Dump(oneInit, oneInit + 20);
        }
        else
        {
            // std::cout << "PLAYER 2:\n";
            PC = queueB[0];
            queueB.erase(queueB.begin());
            // Dump(twoInit, twoInit + 20);
        }

        // std::cout << PC << std::endl;
        bool isDAT = emulateCycle();
        if (isDAT)
            break;

        if (playerOne && !isDAT)
            queueA.push_back(PC);
        else if (!isDAT)
            queueB.push_back(PC);

        playerOne = !playerOne;
        ++counter;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (counter >= max)
        std::cout << "DRAW!!!\n";
    else
        std::cout << "Player " << (queueB.size() < 1 ? "1" : "2") << " won!!!\n";
}

bool VirtualMachine::emulateCycle()
{
    readOperand(PC);
    switch (memory[PC] & 0xF0)
    {
    case 0x00:
        return true;

    case 0x10:
        MOV();
        break;

    case 0x20:
        OP("ADD");
        break;

    case 0x30:
        OP("SUB");
        break;

    case 0x40:
        OP("MUL");
        break;

    case 0x50:
        OP("DIV");
        break;

    case 0x60:
        OP("MOD");
        break;

    case 0x70:
        JMP();
        break;

    case 0x80:
        JMZ();
        break;

    case 0x90:
        JMN();
        break;

    case 0xA0:
        DJN();
        break;

    case 0xB0:
        std::cout << "SPL\n";
        break;

    case 0xC0:
        std::cout << "SLT\n";
        break;

    case 0xD0:
        std::cout << "SEQ\n";
        break;

    case 0xE0:
        std::cout << "SNE\n";
        break;

    case 0xF0:
        std::cout << "NOP\n";
        break;
    }
    PC = mod(PC + 5);
    return false;
}

void VirtualMachine::DJN()
{
    unsigned short A;
    if (A_MODE != 0x00)
    {
        A = read(std::vector<int>{REF_A[0] + 3, 2});
        A = A - 1;
        setOperand(A, REF_A[0] + 3);
    }
    else
    {
        A = read(REF_A);
        A = A - 1;
        setOperand(A, REF_A[0]);
    }

    if (A % 8000 == 0)
        return;

    unsigned short addr;
    if (B_MODE != 0x00)
    {
        unsigned short inc = read(std::vector<int>{PC + 3, 2});
        addr = mod(PC + inc * 5);
    }
    else
        addr = mod(read(REF_B) * 5);

    PC = addr - 5;
}

void VirtualMachine::JMN()
{
    unsigned short A;
    if (A_MODE != 0x00)
        A = read(std::vector<int>{REF_A[0] + 3, 2});
    else
        A = read(REF_A);

    if (A % 8000 == 0)
        return;

    unsigned short addr;
    if (B_MODE != 0x00)
    {
        unsigned short inc = read(std::vector<int>{PC + 3, 2});
        addr = mod(PC + inc * 5);
    }
    else
        addr = mod(read(REF_B) * 5);

    PC = addr - 5;
}

void VirtualMachine::JMZ()
{
    unsigned short A;
    if (A_MODE != 0x00)
        A = read(std::vector<int>{REF_A[0] + 3, 2});
    else
        A = read(REF_A);

    if (A % 8000 != 0)
        return;

    unsigned short addr;
    if (B_MODE != 0x00)
    {
        unsigned short inc = read(std::vector<int>{PC + 3, 2});
        addr = mod(PC + inc * 5);
    }
    else
        addr = mod(read(REF_B) * 5);

    PC = addr - 5;
}

void VirtualMachine::JMP()
{
    unsigned short addr;
    if (B_MODE != 0x00)
    {
        unsigned short inc = read(std::vector<int>{PC + 3, 2});
        addr = mod(PC + inc * 5);
    }
    else
        addr = mod(read(REF_B) * 5);

    PC = addr - 5;
}

void VirtualMachine::OP(std::string op)
{
    if (A_MODE == 0x00)
    {
        // A = number
        if (B_MODE != 0x00)
        {
            unsigned short res = operation(op, (int)read(REF_A), (int)read(std::vector<int>{REF_B[0] + 3, 2}));
            setOperand(res, REF_B[0] + 3);
        }
        else
        {
            unsigned short res = operation(op, (int)read(REF_A), (int)read(std::vector<int>{REF_B}));
            setOperand(res, REF_B[0]);
        }
    }
    else
    {
        if (B_MODE != 0x00)
        {
            unsigned short res = operation(op, (int)read(std::vector<int>{REF_A[0] + 3, 2}), (int)read(std::vector<int>{REF_B[0] + 3, 2}));
            setOperand(res, REF_B[0] + 3);
        }
        else
        {
            unsigned short res = operation(op, (int)read(std::vector<int>{REF_A[0] + 3, 2}), (int)read(std::vector<int>{REF_B}));
            setOperand(res, REF_B[0]);
        }
    }
}

void VirtualMachine::MOV()
{
    if (A_MODE != 0x00)
    {
        // A = address
        if (B_MODE != 0x00)
            setBlock(REF_A, REF_B[0]);
        else
        {
            unsigned short value = (memory[REF_B[0]] << 8) | memory[REF_B[0] + 1];
            setData(value, REF_A[0]);
        }
    }
    else
    {
        // A = number
        if (B_MODE != 0x00)
        {
            unsigned short value = (memory[REF_A[0]] << 8) | memory[REF_A[0] + 1];
            setData(value, REF_B[0] + 3);
        }
        else
            setBlock(REF_A, PC + 3);
    }
}

void VirtualMachine::setOperand(unsigned short data, int target)
{
    memory[target] = (data & 0xFF00) >> 8;
    memory[target + 1] = data & 0x00FF;
}

void VirtualMachine::setData(unsigned short data, int target)
{
    for (int i = 0; i < 3; ++i)
    {
        memory[target + i] = 0;
    }

    memory[target + 3] = (data & 0xFF00) >> 8;
    memory[target + 4] = data & 0x00FF;
}

void VirtualMachine::setBlock(std::vector<int> data, int target)
{
    for (int i = 0; i < data[1]; ++i)
    {
        memory[target + i] = memory[data[0] + i];
    }
}

void VirtualMachine::readOperand(int pc)
{
    unsigned char addrA = (memory[pc] & 0b00001100) >> 2;
    unsigned char addrB = memory[pc] & 0b00000011;
    unsigned short A = (memory[pc + 1] << 8) | memory[pc + 2];
    unsigned short B = (memory[pc + 3] << 8) | memory[pc + 4];

    AAR = pc;
    BAR = pc;
    REF_A = fetchOperand(addrA, A, pc + 1);
    REF_B = fetchOperand(addrB, B, pc + 3);
    A_MODE = addrA;
    B_MODE = addrB;
}

std::vector<int> VirtualMachine::fetchOperand(unsigned char addrMode, unsigned short value, int pointer)
{
    switch (addrMode)
    {
    case 0x00:
        return std::vector<int>{pointer, 2};

    case 0x01:
        return std::vector<int>{mod(PC + (int)value * 5), 5};
    }

    unsigned short referencedB = (fetch(pointer + ((int)value * 5)) << 8) | fetch(pointer + ((int)value * 5) + 1);
    return std::vector<int>{mod(PC + ((int)value * 5) + (int)referencedB * 5), 5};
}

void VirtualMachine::display()
{
    int size = 16;
    int x = 0;
    int y = 0;
    for (int i = 0; i < 8000; ++i)
    {

        bool filled = false;
        for (int j = 0; j < 5; ++j)
        {
            if (memory[i * 5 + j] != 0)
                filled = true;
        }

        if (filled)
        {
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(size, size));
            rectangle.setFillColor(sf::Color::White);
            for (int pointer : queueA)
            {
                if (pointer == i * 5)
                    rectangle.setFillColor(sf::Color::Blue);
            }

            for (int pointer : queueB)
            {
                if (pointer == i * 5)
                    rectangle.setFillColor(sf::Color::Red);
            }

            rectangle.setPosition(x * size, y * size);

            window.draw(rectangle);
        }

        ++x;
        if (x > 90)
        {
            x = 0;
            ++y;
        }
    }
}