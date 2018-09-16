#include "SilentCleanup.hpp"
using namespace Silent;
typedef unsigned long long uint64;

#define DEBUG 1

void freeVariable(SilentVariable* variable)
{
    #if DEBUG
    std::cout << "Cleaning variable:" << variable->name << "\n";
    #endif
    #if DEBUG
    std::cout << "Done cleaning variable:" << variable->name << "\n";
    #endif
    free(variable);
}

void freeStatement(SilentStatement* statement)
{

}

void freeLocalScope(SilentLocalScope* scope)
{
    #if DEBUG
    std::cout << "Cleaning local scope\n";
    #endif
    for(SilentVariable* var : scope->variables) freeVariable(var);
    for(SilentStatement* statement : scope->statements)freeStatement(statement);
    #if DEBUG
    std::cout << "Done cleaning local scope\n";
    #endif
    free(scope);
}

void freeStructure(SilentStructure* structure)
{
    #if DEBUG
    std::cout << "Cleaning structure:" << structure->name << "\n";
    #endif
    for(SilentVariable* var : structure->variables->variables)freeVariable(var);
    #if DEBUG
    std::cout << "Done cleaning structure:" << structure->name << "\n";
    #endif
    free(structure);
}

void freeFunction(SilentFunction* function)
{
    #if DEBUG
    std::cout << "Cleaning function:" << function->name << "\n";
    #endif
    freeLocalScope(function->parameters);
    if(function->initialised) freeLocalScope(function->scope);
    #if DEBUG
    std::cout << "Done cleaning function:" << function->name << "\n";
    #endif
    free(function);
}

void freeNamespace(SilentNamespace* scope)
{
    #if DEBUG
    std::cout << "Cleaning namespace:" << scope->name << "\n";
    #endif
    for(SilentNamespace* name : scope->namespaces) freeNamespace(name);
    for(SilentFunction* function : scope->functions) freeFunction(function);
    for(SilentStructure* structure : scope->types) freeStructure(structure);
    freeLocalScope(scope->globals);
    #if DEBUG
    std::cout << "Done cleaning namespace:" << scope->name << "\n";
    #endif
    free(scope);
}

void Silent::SilentCleanup(SilentParserInfo* info)
{
    #if DEBUG
    std::cout << "Cleaning up...\n";
    #endif
    for(SilentNamespace* globalNamespace : info->namespaces)
    {
        freeNamespace(globalNamespace);
    }
    free(info);
    #if DEBUG
    std::cout << "Cleanup successfull\n";
    #endif
}