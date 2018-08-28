#include "SilentCodeGen.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

std::string transformFunction(SilentFunction& function)
{
    std::string output;
    #if DEBUG
        std::cout << "Transforming function:" << function.name << "\n";
    #endif



    #if DEBUG
        std::cout << "Done transforming function:" << function.name << "\n";
    #endif
    return output;
}

std::string transformNamespace(SilentNamespace& scope)
{
    std::string output;
    #if DEBUG
        std::cout << "Transforming namespace:" << scope.name << "\n";
    #endif

    #if DEBUG
        std::cout << "Done transforming namespace:" << scope.name << "\n";
    #endif
    return output;
}

SilentIntCode* Silent::SilentTransform(SilentParserInfo* parsedCode)
{
    #if DEBUG
        std::cout << "Generating intermediate code...\n";
    #endif

    SilentIntCode* output = new SilentIntCode();
    output->code = "goto main\n";

    
    output->code += transformFunction(*parsedCode->main);

    #if DEBUG
        std::cout << "Done generating intermediate code...\n";
        std::cout << "Generated code:\n" << output->code;
    #endif
    return output;
}