#include "SilentCodeGen.hpp"
#include "SilentParser.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

std::string currentToken;
std::string currentIntNamespace;
std::string currentFunction;
std::vector<std::string> scope;
std::unordered_map<std::string, uint64> symTable;
uint64 cp;

std::string compileFunction()
{
    std::string output;
    return output;
}

std::string compileNamespace(SilentNamespace& scope)
{
    std::string output;
    return output;
}

std::string Silent::SilentCompileAST(SilentParserInfo& info)
{
    cp = 0;
    std::string output;
    output += compileNamespace(*info.globalNamespace);
    return output;
}