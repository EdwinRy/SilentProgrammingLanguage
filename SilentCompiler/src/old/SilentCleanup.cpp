#include "SilentCleanup.hpp"
typedef unsigned long long uint64;
#define DEBUG 0
namespace Silent
{
    void SilentCleaner::FreeVariable(Structures::Variable* variable)
    {
        #if DEBUG
        std::cout << "Cleaning variable:" << variable->name << "\n";
        #endif
        if(variable != NULL) free(variable);
        #if DEBUG
        std::cout << "Done cleaning variable\n";
        #endif
    }

    void SilentCleaner::FreeLocalScope(Structures::LocalScope* scope)
    {
        #if DEBUG
        std::cout << "Cleaning local scope\n";
        #endif
        //for(Structures::Variable* var : scope->variables) FreeVariable(var);
        // for(SilentStatement* statement : scope->statements)
        // freeStatement(statement);
        free(scope);
        #if DEBUG
        std::cout << "Done cleaning local scope\n";
        #endif
    }

    void SilentCleaner::FreeStructure(Structures::Structure* structure)
    {
        #if DEBUG
        std::cout << "Cleaning structure:" << structure->name << "\n";
        #endif
        //for(Structures::Variable* var : structure->members->variables)
        //    FreeVariable(var);
        #if DEBUG
        std::cout << "Done cleaning structure\n";
        #endif
        free(structure);
    }

    void SilentCleaner::FreeFunction(Structures::Function* function)
    {
        #if DEBUG
        std::cout << "Cleaning function:" << function->name << "\n";
        #endif
        //FreeLocalScope(function->parameters);
        //if(function->initialised) FreeLocalScope(function->scope);
        //if(function->scope != NULL) FreeLocalScope(function->scope);
        free(function);
        #if DEBUG
        std::cout << "Done cleaning function\n";
        #endif
    }
    
    void SilentCleaner::FreeNamespace(Structures::Namespace* scope)
    {
        #if DEBUG
        std::cout << "Cleaning namespace:" << scope->name << "\n";
        #endif
        //for(Structures::Namespace* name : scope->namespaces) FreeNamespace(name);
        //for(Structures::Function* function : scope->functions) FreeFunction(function);
        //for(Structures::Structure* structure : scope->types) FreeStructure(structure);
        //if(scope->globals != NULL) FreeLocalScope(scope->globals);
        free(scope);
        #if DEBUG
        std::cout << "Done cleaning namespace\n";
        #endif
    }

    void SilentCleaner::CleanupParser(Parser* parser)
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