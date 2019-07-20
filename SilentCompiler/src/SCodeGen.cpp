#include "SCodeGen.hpp"
namespace Silent
{
    char* CodeGenerator::Compile(Program* ast)
    {
        symTable = ast->table;
        return NULL;
    }

    bool CodeGenerator::CompileProgram(Program* program)
    {
        for (Function* f : program->functions)
            CompileFunction(f);

        for (Namespace* n : program->namespaces)
            CompileNamespace(n);
        return true;
    }

    bool CodeGenerator::CompileNamespace(Namespace* scope)
    {
        std::cout << scope->id << "\n";
        return true;
    }

    bool CodeGenerator::CompileFunction(Function* function)
    {
        return true;
    }
}