#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <bitset>
#include <SFML/Graphics.hpp>

#include "Machine/machine.h"

std::string readFile(std::string fileName)
{
    std::ifstream binFile;
    binFile.open(fileName);

    std::stringstream strStream;
    strStream << binFile.rdbuf();      // read the file
    std::string str = strStream.str(); // str holds the content of the file

    binFile.close();

    return str;
}

std::vector<unsigned char> parseFile(std::string fileName)
{
    std::string contents = readFile(fileName);
    std::vector<unsigned char> binData;

    if (contents == "")
        return binData;

    int bit = 0;
    std::string buffer;
    for (int i = 0; i < contents.length(); ++i)
    {
        buffer += contents[i];
        ++bit;
        if (bit == 8)
        {
            // binData.push_back((unsigned char)&buffer);
            binData.push_back(std::bitset<8>(buffer).to_ullong());
            // binData.push_back(buffer);
            bit = 0;
            buffer = "";
        }
    }

    return binData;
}

bool isOverlap(int a, int sizeA, int b, int sizeB)
{
    if (a <= b && a + sizeA >= b || b <= a && b + sizeB >= a)
        return true;

    return false;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Expected 2 args, programA & programB\n";
        return 1;
    }

    std::string progAName = std::string(argv[1]);
    std::string progBName = std::string(argv[2]);

    std::vector<unsigned char> progA = parseFile(progAName);
    std::vector<unsigned char> progB = parseFile(progBName);

    if (progA.size() == 0 || progB.size() == 0)
    {
        std::cout << "File does not exist/empty file\n";
        return 1;
    }

    VirtualMachine machine;

    srand(time(NULL));

    int startA = (rand() % (8000 - progA.size())) * 5;
    int startB = (rand() % (8000 - progB.size())) * 5;

    while (isOverlap(startA, progA.size(), startB, progB.size()))
    {
        startB = (rand() % (8000 - progB.size())) * 5;
    }

    machine.Load(progA, startA);
    machine.queueA.push_back(startA);

    machine.Load(progB, startB);
    machine.queueB.push_back(startB);

    machine.startEmulation();

    return 0;
}

int testDraw()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML WORKS!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}