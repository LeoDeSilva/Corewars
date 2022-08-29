#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <sstream>

#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "token.h"
#include "Parser/instruction.h"
#include "Compiler/compiler.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Missing File Name, arg[0]\n";
        return 1;
    }

    std::string fileName = std::string(argv[1]);

    std::ifstream inFile;
    inFile.open(fileName); // open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf();       // read the file
    std::string str = strStream.str(); // str holds the content of the file

    if (str == "")
    {
        std::cout << "File Does Not Exist / Empty File\n";
        return 1;
    }

    Lexer lexer(str);
    std::vector<Token> tokens = lexer.tokenise();

    Parser parser(tokens);
    std::vector<Instruction> program = parser.parse();

    Compiler compiler(program);
    std::string binaryString = compiler.compileProgram();

    std::string binFileName = argc >= 3 ? std::string(argv[2]) : std::string("out.bin");
    std::ofstream binFile(binFileName, std::ios::out | std::ofstream::trunc);

    binFile << binaryString;

    return 0;
}