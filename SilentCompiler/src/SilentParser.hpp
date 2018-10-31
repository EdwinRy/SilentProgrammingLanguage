#pragma once
#include <iostream>
#include <vector>
#include "SilentTokenizer.hpp"
namespace Silent
{
    enum class SilentStatementType
    {
        VarInit,
        Expression,
        If,
        Else,
        While,
        For
    };

    enum class SilentPrimitives
    {
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32,
        int64,
        uint64,
        float32,
        float64,
        string,
        pointer,
        null,
        undefined
    };

    enum class SilentOperandType
    {
        Assign,
        Add,
        Subtract,
        Multiply,
        Divide,
        FunctionCall,
        Number,
        String,
        Variable
    };

    enum class SilentExpressionType
    {
        arithmetic,
        logical,
        memory
    };

    typedef struct SilentVariable SilentVariable;
    typedef struct SilentStructure SilentStructure;
    typedef struct SilentFunction SilentFunction;
    typedef struct SilentOperand SilentOperand;
    typedef struct SilentLocalScope SilentLocalScope;
    typedef struct SilentStatement SilentStatement;
    typedef struct SilentNamespace SilentNamespace;


    typedef struct SilentDataType
    {
        union
        {
            SilentStructure* type;
            SilentPrimitives primitive;
        };
        bool isPrimitive;
    }SilentDataType;

    typedef struct SilentVariable
    {
        unsigned long long size;
        unsigned long long localPos;
        std::string name;
        //SilentOperand* expresion;
        SilentDataType type;
        bool initialised;
    }SilentVariable;

    typedef struct SilentStructure
    {
        SilentLocalScope* variables;
        std::string name;
        unsigned long long size;
        bool initialised;
    }SilentStructure;

    typedef struct SilentLocalScope
    {
        std::vector<SilentVariable*> variables;
        std::vector<SilentStatement*> statements;
        bool usesScopeParent;
        union
        {
            SilentLocalScope* scopeParent;
            SilentNamespace* namespaceParent;
        };
    }SilentLocalScope;

    typedef struct SilentFunction
    {
        SilentLocalScope* parameters;
        SilentLocalScope* scope;
        std::string name;
        unsigned long long returnTypePtr;
        unsigned long long returnSize;
        SilentDataType returnType;
        bool initialised;
    }SilentFunction;

    typedef struct SilentOperand
    {
        SilentOperandType type;
        SilentOperand *left, *right;
        union
        {
            SilentToken* token;
            SilentVariable* variable;
        };
    }SilentOperant;

    typedef struct SilentIfStatement
    {
        SilentOperand* expression;
        SilentLocalScope* scope;
    }SilentIfStatement;

    typedef struct SilentWhileLoop
    {
        SilentOperand* expression;
        SilentLocalScope* scope;
    }SilentWhileLoop;

    typedef struct SilentStatement
    {
        SilentLocalScope* parentScope;
        SilentStatementType type;
        union
        {
            SilentIfStatement* ifStatement;
            SilentWhileLoop* whileLoop;
            SilentVariable* variable;
            SilentOperand* expression;
        };
        
    }SilentStatement;

    typedef struct SilentNamespace
    {
        SilentNamespace* parent;
        std::vector<SilentNamespace*> namespaces;
        std::vector<SilentFunction*> functions;
        std::vector<SilentStructure*> types;
        SilentLocalScope* globals;
        std::string name;
    }SilentNamespace;

    class SilentParser
    {
        public:
        SilentParser(); ~SilentParser();
        bool SilentParse(std::vector<Silent::SilentToken> tokens);
        SilentNamespace* GetGlobalNamespace();

        private:
        void ErrorMsg(std::string msg);
        SilentNamespace* GetNamespace(std::string name);
        SilentStructure* GetStructure(std::string name);
        SilentDataType GetType(std::string name);
        SilentFunction* GetLocalFunction(std::string name);
        SilentFunction* GetFunction(std::string name);
        uint64 GetTypeSize(std::string name);
        bool IsPrimitive(std::string name);
        bool IsValidType(std::string name);
        uint64 GetLocalPos(SilentLocalScope &scope);
        SilentVariable* GetLocalVariable(
            SilentLocalScope &scope, std::string name);
        void NextToken();
        SilentToken PeakToken();
        bool AcceptToken(SilentTokenType type);
        bool ExpectToken(SilentTokenType type, std::string msg);
        SilentOperand* ParseFactor();
        SilentOperand* ParseTerm();
        SilentOperand* ParseExpression();
        //SilentStatement* ParseStatement(SilentLocalScope &scope);
        SilentVariable* ParseVariable(
            SilentLocalScope &scope, bool init, bool expectEnd);
        SilentStructure* ParseStruct(SilentNamespace &scope);
        SilentLocalScope* ParseParameters(SilentNamespace &scope);
        SilentLocalScope* ParseLocalScope(SilentNamespace &scope);
        SilentFunction* ParseFunction(SilentNamespace& scope);
        SilentNamespace* ParseNamespace(SilentNamespace &scope);

        SilentNamespace* globalNamespace;
        SilentToken ct; //Current token
        uint64 tokenCursor;
        std::vector<SilentNamespace*> accessibleNamespaces;
        std::vector<Silent::SilentToken> *tokensPtr;

        
    };
}