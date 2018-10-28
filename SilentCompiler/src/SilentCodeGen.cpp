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

// std::string Silent::SilentCompileAST(SilentParserInfo& info)
// {
//     cd.cp = 0;
//     std::string output = "";
//     output += compileNamespace(*info.globalNamespace);
//     return output;
// }

namespace Silent
{
    void SilentCodeGenerator::Compile(SilentParser* parser)
    {
        cp = 0;
        code = "";
        code += CompileNamespace(*parser->GetGlobalNamespace());
    }

    std::string SilentCodeGenerator::GetOutput() {return code;}

    std::string SilentCodeGenerator::GenScopeName(std::string id)
    {
        std::string name = "";
        for(SilentNamespace *scope : namespaces)
        {
            name += scope->name;
            name += "::";
        }
        name += id;
        return name;
    }
    std::string SilentCodeGenerator::CompileExpression(
        SilentOperand &expression
    )
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
    std::string SilentCodeGenerator::CompileFunction(SilentFunction &func)
    {
        std::string output;
        #if DEBUG
        std::cout << "Compiling function: " << func.name << "\n";
        #endif

        std::cout << "Scope name: " << GenScopeName(func.name) << "\n";

        #if DEBUG
        std::cout << "Done compiling function: " << func.name << "\n";
        #endif
        return output;
    }
    std::string SilentCodeGenerator::CompileNamespace(SilentNamespace &scope)
    {
        std::string output = "";
        #if DEBUG
        std::cout << "Compiling namespace: " << scope.name << "\n";
        #endif

        namespaces.push_back(&scope);

        //Compile nested namespace
        for(SilentNamespace* nScope : scope.namespaces)
            output += CompileNamespace(*nScope);

        //Compile namespace functions
        for(SilentFunction* func : scope.functions)
            output += CompileFunction(*func);

        namespaces.pop_back();

        #if DEBUG
        std::cout << "Done compiling namespace: " << scope.name << "\n";
        #endif
        return output;
    }
}