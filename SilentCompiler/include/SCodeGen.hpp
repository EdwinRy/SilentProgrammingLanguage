#include "SParser.hpp"
#include <vector>
#pragma once

namespace Silent
{
    class CodeGenerator
    {
        public:
        char* Compile(Program* ast);

        private:
        SymbolTable* symTable;
        std::vector<char> code;
        char* bytecodeOut = NULL;
        
        
        // Compile functions
        bool CompileProgram(Program* program);
        bool CompileNamespace(Namespace* scope);
        bool CompileFunction(Function* function);

        // Helper functions

        
    };
}