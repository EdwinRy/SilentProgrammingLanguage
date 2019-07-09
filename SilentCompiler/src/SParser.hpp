#include <iostream>
#include <string.h>
#include <vector>
#include "STokenizer.hpp"
#include <unordered_map>
#pragma once

namespace Silent
{

    class Program;
    class Namespace;
    class Function;

    class TableNode
    {
        public:
        enum class NodeType
        {
            Namespace,
            Function,
            Structure,
            Variable,
            Literal,
            Expression
        }nodeType;

        template <class T>
        TableNode(T* node, NodeType type);
        TableNode(){}
        void* GetNode(){return node;}

        private:
        // ScopeResolution scopeRes;
        void* node;
    };

    class SymbolTable
    {
        public:
        void AddItem(TableNode node);
        void AddChild(SymbolTable child);
        TableNode GetItemBack(){return items.back();}
        void SetParent(SymbolTable* parent);
        void SetupChild();
        void AddCurrentChild();
        SymbolTable* GetParent();

        static thread_local SymbolTable* currentTable;

        private:
        std::vector<SymbolTable> children;
        SymbolTable* parent;

        std::vector<TableNode> items;
    };

    class Parser
    {
        public:
        Parser();
        Program* Parse(std::vector<Silent::Token> tokensPtr);
        void ErrorMsg(std::string msg);
        void WarningMsg(std::string msg);

        Token GetToken();
        uint64_t GetTokenCursor();
        Token NextToken();
        Token NextToken(int offset);
        Token PeakToken();
        Token PeakToken(int offset);
        Token PreviousToken();
        Token PreviousToken(int offset);
        void Revert(uint64_t revertPtr);
        bool Match(TokenType type);
        bool IsType(TokenType type);
        std::string GetTokenVal();
        TokenType GetTokenType();

        private:
        unsigned long long errorCount;
        unsigned long long warningCount;
        Token ct;
        unsigned long long tokenCursor;
        std::vector<Silent::Token> *tokensPtr;
    };

    class Type
    {
        public:
        bool Parse(Parser &parser);
        class Name
        {
            public:
            bool Parse(Parser &parser);
            enum class Type
            {
                Primitive,
                Identifier,
                AccessIdentifier
            }type;
            std::string value;
        };
    };


    class Program
    {
        public:
        bool Parse(Parser &parser);
        std::vector<Namespace> namespaces;
        std::vector<Function> functions;
        std::vector<Type> types;
        SymbolTable table;
    };

    class Namespace
    {
        public:
        bool Parse(Parser &parser);

        private:
        bool ParseDeclaration(Parser &parser);
        bool ParseScope(Parser &parser);
        bool ParseIdentifier(Parser &parser);
        std::vector<Function> functions;
        std::vector<Type> types;
        SymbolTable symTable;
        std::string id;
    };

    class VariableDeclaration
    {
        public:
        bool Parse(Parser &parser);

        private:
        Type::Name type;
        std::string id;
    };

    class Operand
    {
        public:
        enum class Type
        {
            And,
            Or,
            Xor,

            Add,
            Sub,
            Mul,
            Div,
            None,

            Literal,
            Assign,
            AddAssign,
            SubtractAssign,
            MultiplyAssign,
            DivideAssign,

            Factor,
            Term,
            Sum,
            Logic,
            Comparison,
            Conditional,
            Expression
        }type;
        static Operand* ParseFactor(Parser &parser);
        static Operand* ParseTerm(Parser &parser);
        static Operand* ParseSum(Parser &parser);
        static Operand* ParseLogic(Parser &parser);
        static Operand* ParseComparison(Parser &parser);
        static Operand* ParseConditional(Parser &parser);
        static Operand* ParseAssignment(Parser &parser);

        static Type TokenToOperandType(TokenType tokenType);

        Operand *left, *right;
        TableNode node;
    };

    class Expression
    {
        public:
        bool Parse(Parser &parser);
        private:
        Operand *op;
    };

    class Function
    {
        public:
        bool Parse(Parser &parser);

        private:
        bool ParseIdentifier(Parser &parser);
        bool ParseParameters(Parser &parser);
        bool ParseScope(Parser &parser);

        SymbolTable symTable;
        std::string id;

        class Parameter
        {
            public:
            bool Parse(Parser &parser);

            private:
            Type::Name type;
            std::string id;
        };

        std::vector<Parameter> params;
        
        class LocalStatement
        {
            public:
            bool Parse(Parser &parser);

            private:
            enum class Type
            {
                VariableDeclaration,
                Expression
            }type;

            VariableDeclaration varDec;
            Expression expression;

        };

        std::vector<LocalStatement> statements;

    };

    class Variable
    {

    };
}