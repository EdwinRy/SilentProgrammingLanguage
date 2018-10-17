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



struct CompilerData
{
    std::string currentToken;
    std::string currentNamespace;
    std::string currentFunction;
    std::vector<std::string> scope;
    std::unordered_map<std::string, uint64> symTable;
    uint64 cp;
};
CompilerData cd;

std::string compileExpression(SilentOperand& expression)
{
    std::string output;
    #if DEBUG
        std::cout << "Compiling expression\n";
    #endif

    #if DEBUG
        std::cout << "Done compiling  expression\n";
    #endif
    return output;
}

std::string compileFunction(SilentFunction& func)
{
    std::string output;
    #if DEBUG
        std::cout << "Compiling function: " << func.name << "\n";
    #endif

    #if DEBUG
        std::cout << "Done compiling function: " << func.name << "\n";
    #endif
    return output;
}

std::string compileNamespace(SilentNamespace& scope)
{
    std::string output;
    #if DEBUG
        std::cout << "Compiling namespace: " << scope.name << "\n";
    #endif



    #if DEBUG
        std::cout << "Done compiling namespace: " << scope.name << "\n";
    #endif
    return output;
}

std::string Silent::SilentCompileAST(SilentParserInfo& info)
{
    cd.cp = 0;
    std::string output;
    output += compileNamespace(*info.globalNamespace);
    return output;
}