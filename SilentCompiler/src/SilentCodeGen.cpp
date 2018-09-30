#include "SilentCodeGen.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

std::string currentToken;
std::string currentNamespace;
std::string currentFunction;
std::vector<std::string> scope;
std::string inputCode;
uint64 cp; // intermediate code pointer

void nextToken()
{
    std::string buffer = "";
    for(;inputCode[cp] != ' '; cp++) { buffer += cp; }
    cp++;
    currentToken = buffer;
}

std::string compileFunction()
{

}

std::string compileNamespace()
{

}

std::string Silent::SilentCompileIntCode(std::string intCode)
{
    cp = 0;
    inputCode = intCode;

    std::string output;



    return output;
}