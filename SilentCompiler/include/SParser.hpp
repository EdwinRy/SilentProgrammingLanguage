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
    class Type;
    class VariableDeclaration;
    class Method;
    class Attribute;

    class TableNode
    {
        public:
        enum class Type
        {
            Program,
            Namespace,
            Function,
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
        void* GetNode(){return node;}

        private:
        void* node;
    };

    class SymbolTable
    {
        public:
        SymbolTable();
        void AddItem(TableNode node);
        void AddChild(SymbolTable *child);
        TableNode GetItemBack(){return items.back();}
        void SetParent(SymbolTable* parent);
        SymbolTable* GetParent();

        static thread_local SymbolTable* currentTable;
        TableNode self;

        private:
        SymbolTable* parent;
        std::vector<SymbolTable*> children;
        std::vector<TableNode> items;
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
        std::vector<Function*> functions;
        std::vector<Type*> types;
        SymbolTable* table;
    };

    class Namespace
    {
        public:
        bool Parse(Parser &parser);

        private:
        friend class CodeGenerator;
        bool ParseDeclaration(Parser &parser);
        bool ParseScope(Parser &parser);
        bool ParseIdentifier(Parser &parser);
        std::vector<Function*> functions;
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
        Type(){}
        Type(std::string id, uint64_t size);
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
                    Method* method;
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
        uint64_t size;
        std::vector<Member*> members;
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

    class VariableDeclaration
    {
        public:
        bool Parse(Parser& parser);

        private:
        friend class CodeGenerator;
        TypeName type;
        std::string id;
        Expression init;
    };

    class Function
    {
        public:
        bool Parse(Parser &parser);

        protected:
        friend class CodeGenerator;
        bool ParseIdentifier(Parser &parser);
        bool ParseParameters(Parser &parser);
        bool ParseScope(Parser &parser);

        SymbolTable *symTable;
        std::string id;

        class Parameter
        {
            public:
            bool Parse(Parser &parser);

            private:
            TypeName type;
            std::string id;
        };

        std::vector<Parameter*> params;
        
        class LocalStatement
        {
            public:
            bool Parse(Parser &parser);

            private:
            friend class CodeGenerator;
            enum class Type
            {
                VariableDeclaration,
                Expression
            }type;

            union
            {
                VariableDeclaration *varDec;
                Expression *expression;
            };

        };

        std::vector<LocalStatement> statements;

    };

    class Method : Function
    {
        public:
        bool Parse(Parser& parser);
        void SetParent(Type* type);

        private:
        Type* self;
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
    
}