#include "SParser.hpp"
#pragma once
namespace Silent
{
namespace Semantics
{
    using Parser::TableNode;
    using Parser::SymbolTable;

    class SemanticAnalyser
    {
        public:
        bool Analyse(
            SymbolTable *symbolTable, 
            Parser::Program *program);

        private:

        SymbolTable* symTable;
        Parser::Program* prog;
    };
    thread_local SemanticAnalyser* analyser;

    class Program
    {
        public:
        bool Analyse(Parser::Program* prog);


        private:
        friend class Semanticanalyser;
        std::vector<Parser::Namespace*> namespaces;
        std::vector<Parser::Subroutine*> functions;
        std::vector<Parser::Type*> types;
        SymbolTable* table;
    };

    class Namespace
    {
        public:
        bool Analyse(Parser::Namespace* scope);

        private:
        friend class Semanticanalyser;
    };

    class Type
    {
        public:
        bool Analyse(Parser::Type* type);
        private:
        friend class Semanticanalyser;
    };

    class Subroutine
    {
        public:
        bool Analyse(Parser::Subroutine* sub);
        private:
        friend class Semanticanalyser;
    };

    class Variable
    {
        public:
        bool Analyse(Parser::Variable* var);
        private:
        friend class Semanticanalyser;
    };
}}