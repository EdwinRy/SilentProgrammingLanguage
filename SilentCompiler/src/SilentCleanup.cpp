#include "SilentCleanup.hpp"
typedef unsigned long long uint64;
#define DEBUG 0

namespace Silent
{
    void SilentCleaner::FreeVariable(SilentVariable* variable)
    {
        #if DEBUG
        std::cout << "Cleaning variable:" << variable->name << "\n";
        #endif
        free(variable);
        #if DEBUG
        std::cout << "Done cleaning variable:" << variable->name << "\n";
        #endif
    }
    void SilentCleaner::FreeLocalScope(SilentLocalScope* scope)
    {
        #if DEBUG
        std::cout << "Cleaning local scope\n";
        #endif
        for(SilentVariable* var : scope->variables) FreeVariable(var);
        //for(SilentStatement* statement : scope->statements)
        //freeStatement(statement);
        free(scope);
        #if DEBUG
        std::cout << "Done cleaning local scope\n";
        #endif
    }
    void SilentCleaner::FreeStructure(SilentStructure* structure)
    {
        #if DEBUG
        std::cout << "Cleaning structure:" << structure->name << "\n";
        #endif
        for(SilentVariable* var : structure->variables->variables)
            FreeVariable(var);
        free(structure);
        #if DEBUG
        std::cout << "Done cleaning structure:" << structure->name << "\n";
        #endif
    }
    void SilentCleaner::FreeFunction(SilentFunction* function)
    {
        #if DEBUG
        std::cout << "Cleaning function:" << function->name << "\n";
        #endif
        FreeLocalScope(function->parameters);
        if(function->initialised) FreeLocalScope(function->scope);
        free(function);
        #if DEBUG
        std::cout << "Done cleaning function:" << function->name << "\n";
        #endif
    }
    void SilentCleaner::FreeNamespace(SilentNamespace* scope)
    {
        #if DEBUG
        std::cout << "Cleaning namespace:" << scope->name << "\n";
        #endif
        for(SilentNamespace* name : scope->namespaces) FreeNamespace(name);
        for(SilentFunction* function : scope->functions) FreeFunction(function);
        for(SilentStructure* structure : scope->types) FreeStructure(structure);
        if(scope->globals != NULL) FreeLocalScope(scope->globals);
        #if DEBUG
        std::cout << "Done cleaning namespace:" << scope->name << "\n";
        #endif
        free(scope);
    }

    void SilentCleaner::CleanupParser(SilentParser* parser)
    {
        #if DEBUG
        std::cout << "Cleaning up...\n";
        #endif
        FreeNamespace(parser->GetGlobalNamespace());
        #if DEBUG
        std::cout << "Cleanup successfull\n";
        #endif
    }
}