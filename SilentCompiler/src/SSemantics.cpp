#include "SSemantics.hpp"

namespace Silent
{
namespace Semantics
{
    bool SemanticAnalyser::Analyse(
        SymbolTable* symbolTable, 
        Parser::Program* program)
    {
        analyser = this;
        return true;
    }

    bool Program::Analyse(Parser::Program* prog)
    {
        this->namespaces = prog->namespaces;
        this->functions = prog->functions;
        this->types = prog->types;

        for(auto& scope : prog->namespaces)


        return true;
    }

}}