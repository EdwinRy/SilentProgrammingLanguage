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
    //std::string currentNamespace;
    std::string currentFunction;
    std::vector<SilentNamespace*> currentNamespace;
    std::unordered_map<std::string, uint64> symTable;
    uint64 cp;
};
CompilerData cd;

std::string genScopeName(std::string id)
{
    std::string name = "";

    for(SilentNamespace* scope : cd.currentNamespace)
    {
        name += scope->name; name += ":";
    }

    name += id;
    return name;
}

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

    std::cout << "Scope name: " << genScopeName(func.name) << "\n";

    #if DEBUG
        std::cout << "Done compiling function: " << func.name << "\n";
    #endif
    return output;
}

std::string compileNamespace(SilentNamespace& scope)
{
    std::string output = "";
    #if DEBUG
        std::cout << "Compiling namespace: " << scope.name << "\n";
    #endif

    cd.currentNamespace.push_back(&scope);

    //Compile nested namespace
    for(SilentNamespace* nScope : scope.namespaces)
        output += compileNamespace(*nScope);

    //Compile namespace functions
    for(SilentFunction* func : scope.functions)
        output += compileFunction(*func);


    cd.currentNamespace.pop_back();

    #if DEBUG
        std::cout << "Done compiling namespace: " << scope.name << "\n";
    #endif
    return output;
}

std::string Silent::SilentCompileAST(SilentParserInfo& info)
{
    cd.cp = 0;
    std::string output = "";
    output += compileNamespace(*info.globalNamespace);
    return output;
}