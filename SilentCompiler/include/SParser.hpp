#include <iostream>
#include <string.h>
#include <vector>
#include "STokenizer.hpp"
#include <unordered_map>
#pragma once

namespace Silent {
namespace Parser
{
    class Program;
    class Namespace;
    class Type;
    class Variable;
    class Subroutine;
    class Attribute;
    class Parameter;

    class TableNode
    {
        public:
        enum class Type
        {
            Program,
            Namespace,
            Subroutine,
            Structure,
            Variable,
            Literal,
            Expression,
            ExpressionLHS,
            Null
        }nodeType;

        template <class T>
        TableNode(T* node, Type type);
        TableNode();
        
        private:
        void* node;

        // Getters and Setters
        public:
        void* GetNode() { return node; }
    };

    class SymbolTable
    {
        public:
        SymbolTable();
        void AddItem(TableNode node);
        void AddChild(SymbolTable *child);

        static thread_local SymbolTable* currentTable;
        TableNode self;

        private:
        SymbolTable* parent;
        std::vector<SymbolTable*> children;
        std::vector<TableNode> items;

        public:
        SymbolTable* GetParent();
        TableNode GetItemBack() { return items.back(); }
        std::vector<TableNode> GetItems() { return items; }

        void SetParent(SymbolTable* parent);
    };

    class Parser
    {
        public:
        Parser();
        Program* Parse(std::vector<Silent::Token> tokensPtr);
        void ErrorMsg(std::string msg);
        void WarningMsg(std::string msg);
        void DebugMsg(std::string msg);

        Token GetToken();
        uint64_t GetTokenCursor();
        Token NextToken();
        Token NextToken(uint64_t offset);
        Token PeakToken();
        Token PeakToken(uint64_t offset);
        Token PreviousToken();
        Token PreviousToken(uint64_t offset);
        void Revert(uint64_t revertPtr);
        bool Match(TokenType type);
        bool IsType(TokenType type);
        std::string GetTokenVal();
        TokenType GetTokenType();

        private:
        bool errorsEnabled;
        bool warningsEnabled;
        bool debuggingEnabled;
        unsigned long long errorCount;
        unsigned long long warningCount;
        Token ct;
        unsigned long long tokenCursor;
        std::vector<Silent::Token> *tokensPtr;
    };


    class Program
    {
        public:
        bool Parse(Parser &parser);
        std::vector<Namespace*> namespaces;
        std::vector<Subroutine*> functions;
        std::vector<Type*> types;
        SymbolTable* table;
    };

    class Namespace
    {
        public:
        bool Parse(Parser &parser);
        std::string GetId() { return id; }
        SymbolTable* GetTable() { return symTable; }

        private:
        friend class CodeGenerator;
        bool ParseDeclaration(Parser &parser);
        bool ParseScope(Parser &parser);
        bool ParseIdentifier(Parser &parser);
        std::vector<Subroutine*> functions;
        std::vector<Type*> types;
        SymbolTable *symTable;
        std::string id;
    };

    class TypeName
    {
        public:
        bool Parse(Parser& parser);
        enum class Type
        {
            Primitive,
            Identifier,
            AccessIdentifier
        }type;
        std::string value;
    };

    class Type
    {
        public:
        bool Parse(Parser &parser);
        bool ParseIdentifier(Parser& parser);
        bool ParseScope(Parser& parser);

        class Member
        {
            public:
                enum class MemberType
                {
                    Attribute,
                    Method
                };
                MemberType memberType;

                union
                {
                    Attribute* attribute;
                    Subroutine* method;
                };

                enum class AccessModifier
                {
                    Public,
                    Private,
                    Protected
                };
                AccessModifier accessModifier;
                
                Type* parentType;
                bool Parse(Parser& parser, Type* parent);
                static AccessModifier TokenToAccess(TokenType type);
                static AccessModifier currentAccessModifier;
        };

        private:
        SymbolTable* symTable;
        friend class CodeGenerator;
        std::string id;
        std::vector<Member*> members;

        public:
        std::string GetId() { return id; }
        SymbolTable* GetTable() { return symTable; }
    };

    

    class Literal
    {
        enum class Type
        {
            Integer,
            Float,
            String,
            None
        }type = Type::None;

        public:
        bool Parse(Parser& parser);
        std::string val;
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

            Assign,
            AddAssign,
            SubtractAssign,
            MultiplyAssign,
            DivideAssign,

            //Literal,
            //Expression,
            //ExpressionLHS,
            //FunctionCall,
            Factor,
            None,
        }type = Type::None;
        static Operand* ParseFactor(Parser &parser);
        static Operand* ParseTerm(Parser &parser);
        static Operand* ParseSum(Parser &parser);
        static Operand* ParseLogic(Parser &parser);
        static Operand* ParseComparison(Parser &parser);
        static Operand* ParseConditional(Parser &parser);
        static Operand* ParseAssignment(Parser &parser);

        static Type TokenToOperandType(TokenType tokenType);

        Operand *left = NULL, *right = NULL;
        TableNode node;
    };

    class Expression
    {
        public:
        bool Parse(Parser &parser);
        private:
        friend class CodeGenerator;
        Operand *op;
    };

    class ExpressionLHS
    {
        public:
        bool Parse(Parser& parser);

        class ObjectAccess
        {
            public:
            bool Parse(Parser& parser);
            std::vector<std::string> access;
        }*objectAccess;

        class ArrayAccess
        {
            public:
            bool Parse(Parser& parser);
            std::vector<Expression> access;
        }*arrayAccess;

        private:
        std::string id;
    };

    class Variable
    {
        public:
        Variable(){}
        Variable(Parameter* param);
        bool Parse(Parser& parser);
        

        private:
        friend class CodeGenerator;
        TypeName type;
        std::string id;
        Expression init;
        uint64_t scopeIndex;

        public:
        std::string GetId() { return id; }
    };

    class LocalStatement
    {
        public:
        bool Parse(Parser& parser);

        private:
        friend class CodeGenerator;
        enum class Type
        {
            VariableDeclaration,
            Expression
        }type;

        union
        {
            Variable* varDec;
            Expression* expression;
        };

    };

    class Parameter
    {
        public:
        bool Parse(Parser &parser);

        private:
        friend class Variable;
        TypeName type;
        std::string id;
    };

    class Subroutine
    {
        public:
        bool Parse(Parser &parser);
        std::string GetId() { return id; }
        SymbolTable* GetTable() { return symTable; }

        private:
        friend class CodeGenerator;
        bool ParseIdentifier(Parser &parser);
        bool ParseReturnType(Parser& parser);
        bool ParseParameters(Parser &parser);
        bool ParseScope(Parser &parser);

        SymbolTable *symTable;
        std::string id;

        TypeName returnType;
        std::vector<Parameter*> params;
        std::vector<LocalStatement> statements;

    };



    class Attribute
    {
        public:
        bool Parse(Parser& parser);

        private:
        friend class CodeGenerator;
        TypeName type;
        std::string id;
        
    };
    
}}