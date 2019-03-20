#pragma once
#include "SParser.hpp"
#include <iostream>
#include <vector>
namespace Silent
{
    typedef class Parser parser;
    typedef class ScopeResolution ScopeResolution;
}

namespace Silent::Types
{

    class Namespace
    {
        public:
        static void Parse(Parser &parser, ScopeResolution parent);
        std::string identifier;
        ScopeResolution *scopeResolution;

    };

    class AsmInstruction
    {
        public:
        enum class OpCodeType
        {

        }Opcode;

        std::vector<std::string> values;
    };

    class Asm
    {
        public:
        void Parse(Parser &parser, ScopeResolution parent);

        std::vector<AsmInstruction> instructions;

        
    };

    class Expression
    {

    };

    class Statement
    {
        public:
        void Parse(Parser &parser, ScopeResolution parent);
        enum class Type
        {
            Asm,
            If,
            While,
            Expression
        }StatementType;

        union
        {
            /* data */
        };
        
    };

    class Function
    {
        public:
        static void Parse(Parser &parser, ScopeResolution parent);
        std::string identifier;
        ScopeResolution *scopeResolution;
        ScopeResolution *returnType;

        std::vector<Statement> statements;
    };

    class Variable
    {

    };

    class Type
    {

    };

    class Node
    {
        public:
        enum class Type
        {
            Namespace,
            Class,
            Function,
            Variable


        }nodeType;

        union
        {
            Type *type;
            Function *function;
            Variable *variable;
            Namespace *module;
        };
    };
};