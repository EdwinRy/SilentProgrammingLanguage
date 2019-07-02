#include <iostream>
#include <string.h>
#include <vector>
#include "STokenizer.hpp"
#include <unordered_map>
#pragma once

namespace Silent
{

    class NamespaceDeclaration;
    class FunctionDeclaration;
    class TypeDeclaration;
    class Namespace;
    class Program;


    // class ScopeResolution
    // {
    //     public:
    //     ScopeResolution(){}
    //     ScopeResolution(std::string scope);
    //     ScopeResolution(std::vector<std::string> scope);

    //     std::vector<std::string> scopeList;
    //     std::string scopeFormatted;

    //     ScopeResolution& operator+=(std::string rhs);
    //     bool operator==(const ScopeResolution& rhs) const;
    //     ScopeResolution& operator--();

    // };

    class TableNode
    {
        public:
        enum class NodeType
        {
            Namespace,
            Function,
            Structure,
            LocalScope,
            Variable
        }nodeType;

        template <class Type>
        TableNode(Type* node, NodeType type);
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
        unsigned long long GetTokenCursor();
        Token NextToken();
        Token NextToken(int offset);
        Token PeakToken();
        Token PeakToken(int offset);
        Token PreviousToken();
        Token PreviousToken(int offset);
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


    // Tokens
    class Program
    {
        public:
        bool Parse(Parser &parser);
        std::vector<NamespaceDeclaration> namespaceDeclarations;
        std::vector<FunctionDeclaration> functionDeclarations;
        std::vector<TypeDeclaration> typeDeclarations;
        SymbolTable table;
    };

    class NamespaceScope
    {
        public:
        bool Parse(Parser &parser);

        private:
        std::vector<FunctionDeclaration> functionDeclarations;
        std::vector<TypeDeclaration> typeDeclarations;
        SymbolTable symTable;
    };

    class NamespaceDeclaration
    {
        public:
        bool Parse(Parser &parser);

        private:
        std::string identifier;
        NamespaceScope scope;
    };

    class TypeName
    {
        public:
        bool Parse(Parser &parser);
        enum class Type
        {
            Primitive,
            Identifier,
            AccessIdentifier
        }type;

        private:
        std::string value;
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

        static Type TokenToOperandType(TokenType tokenType);

        Operand *left, *right;
        TableNode node;
    };

    class Float
    {
        public:
        bool Parse(Parser &parser);
        std::string value;
    };

    class Integer
    {
        public:
        bool Parse(Parser &parser);
        std::string value;
        bool isNegative;
    };

    class Literal
    {
        public:
        bool Parse(Parser &parser);
        enum class Type
        {
            Integer,
            Float,
            String
        }type;
        std::string value;
    };

    class ExpressionName
    {
        public:
        bool Parse(Parser &parser);
        std::string name;
    };

    class ExpressionLHS
    {
        enum class Type
        {
            ExpressionName,
            ArrayAccess
        }type;

        ExpressionName expressionName;

    };

    class Assignment
    {

    };

    class FunctionCall
    {

    };

    class Expression
    {
        public:
        bool Parse(Parser &parser);
    };

    class VariableDeclaration
    {
        public:
        bool Parse(Parser &parser);

        private:
        TypeName type;
        std::string id;
    };

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

    class LocalScope
    {
        public:
        bool Parse(Parser &parser);

        private:
        std::vector<LocalStatement> statements;
        SymbolTable symTable;
    };

    

    class Parameter
    {
        public:
        bool Parse(Parser &parser);

        private:
        TypeName type;
        std::string identifier;
        
    };

    class Parameters
    {
        public:
        bool Parse(Parser &parser);
        uint64_t GetParamCount(){return parameters.size();}
        // Get param should be used EXCLUSIVELY in cases where either:
        // The vector is not edited past the return or the pointer is not 
        // to be used after vector's alteration due to memory reallocation
        Parameter* GetParam(uint64_t i){return &parameters[i];}

        private:
        std::vector<Parameter> parameters;
    };

    class FunctionDeclaration
    {
        public:
        bool Parse(Parser &parser);
        Parameters GetParams(){return parameters;}

        private:
        std::string identifier;
        Parameters parameters;
        LocalScope scope;
    };

    class TypeDeclaration
    {
        public:
        bool Parse(Parser &parser);
    };

}