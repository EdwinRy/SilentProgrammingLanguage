#pragma once
#include "SParser.hpp"
#include <iostream>
#include <vector>
namespace Silent
{
    typedef class Parser parser;
    typedef class ScopeResolution ScopeResolution;
}

namespace Silent::SemanticTypes
{
    class Namespace
    {
        public:
        unsigned long long size;
        std::string id;
        
    };
}

namespace Silent::ParserTypes
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
        void Parse(Parser &parser);
        enum class OpcodeType
        {
            Halt = 0,
            Goto,

            Call,
            Return,
            LoadDll,//
            LoadDllFunc,//
            FreeDll,//
            CallDllFunc,//

            Push1,
            Push2,
            Push4,
            Push8,
            PushX,

            Pop1,
            Pop2,
            Pop4,
            Pop8,
            PopX,

            Store1,
            Store2,
            Store4,
            Store8,
            StoreX,

            Load1,
            Load2,
            Load4,
            Load8,
            LoadX,

            StoreGlobal1,
            StoreGlobal2,
            StoreGlobal4,
            StoreGlobal8,
            StoreGlobalX,

            LoadGlobal1,
            LoadGlobal2,
            LoadGlobal4,
            LoadGlobal8,
            LoadGlobalX,

            Alloc1,
            Alloc2,
            Alloc4,
            Alloc8,
            AllocX,

            LoadPtr1,
            LoadPtr2,
            LoadPtr4,
            LoadPtr8,
            LoadPtrX,

            StorePtr1,
            StorePtr2,
            StorePtr4,
            StorePtr8,
            StorePtrX,
            Free,
            GetPtr,

            AddI1,
            AddI2,
            AddI4,
            AddI8,
            AddF4,
            AddF8,

            SubI1,
            SubI2,
            SubI4,
            SubI8,
            SubF4,
            SubF8,

            MulI1,
            MulI2,
            MulI4,
            MulI8,
            MulF4,
            MulF8,

            DivI1,
            DivI2,
            DivI4,
            DivI8,
            DivF4,
            DivF8,

            //Convert,
            SmallerThan,
            SmallerThanOrEqual,
            LargerThan,
            LargerThanOrEqual,
            Equal,
            NotEqual,

            If,
            IfNot,
            And,
            Or,
            Xor,
            Not,
            Label

        }Opcode;

        static OpcodeType StrToOpcode(std::string str);

        std::vector<std::string> values;
    };

    class Asm
    {
        public:
        void Parse(Parser &parser);

        std::vector<AsmInstruction> instructions;
    };

    typedef class Node node;

    class Operand
    {
        public:
        static Operand* ParseFactor(Parser &parser);
        static Operand* ParseTerm(Parser &parser);
        static Operand* ParseSum(Parser &parser);
        static Operand* ParseLogic(Parser &parser);
        static Operand* ParseComparison(Parser &parser);
        static Operand* ParseExpression(Parser &parser);

        enum class OperatorType
        {
            Add,
            Substract,
            Multiply,
            Divide,
            And,
            Or,
            Xor,
            Not,
            None
        }Operator;

        bool operatorOverloaded;

        Operand *left, *right;
        Node* value;
    };

    class Variable
    {
        public:
        ScopeResolution *type;
        std::string identifier;
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
            Expression,
            Declaration,
        }StatementType;

        union
        {
            Asm *assembly;
            Variable *var;
        };
        
    };

    typedef class TypeMember typeMember;

    class Method
    {
        public:
        static void Parse(Parser &parser, ScopeResolution parent);
        static void ParseParameters(Parser &parser, ScopeResolution parent);
    };

    class TypeMember
    {
        public:
        enum class Type
        {
            Method,
            Variable
        }memberType;

        union
        {
            Method* method;
        };

        enum class AccessModifier
        {
            Public,
            Protected,
            Private
        }accessModifier;
    };

    class Type
    {
        public:
        static void Parse(Parser &parser, ScopeResolution parent);
        std::string identifier;
        ScopeResolution *scopeResolution;
    };

    class Function
    {
        public:
        static void Parse(Parser &parser, ScopeResolution parent);
        void ParseParameters(Parser &parser);
        std::string identifier;
        ScopeResolution *scopeResolution;
        ScopeResolution *returnType;

        std::vector<Statement> statements;
        std::vector<Variable> parameters;
    };

    class Node
    {
        public:
        enum class NodeType
        {
            Namespace,
            Type,
            Function,
            Variable,
            Value

        }nodeType;

        union
        {
            Type *type;
            Function *function;
            Variable *variable;
            Namespace *module;
            std::string *strValue;
        };
    };
};