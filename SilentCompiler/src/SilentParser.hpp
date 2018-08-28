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

    enum class SilentDataType
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
        undefined,
        structType
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


    typedef struct SilentVariable
    {
        unsigned long long size;
        unsigned long long localPos;
        std::string name;
        SilentOperand* expresion;
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

    typedef struct SilentParserInfo
    {
        SilentFunction* main;
        std::vector<SilentNamespace*> namespaces;
    }SilentParserInfo;

    SilentStatement* SilentParseStatement(SilentLocalScope &scope);

    SilentVariable* SilentParseVar(
        SilentLocalScope &scope,
        SilentNamespace &typeScope,
        std::string type,
        bool init,
        bool expectEnd
    );

    SilentStructure* SilentParseStruct(SilentNamespace &scope);
    SilentLocalScope* SilentParseParameters(SilentNamespace &scope);
    SilentLocalScope* SilentParseLocalScope(SilentNamespace &scope);
    SilentLocalScope* SilentParseFunctionScope(SilentNamespace& scope);
    SilentFunction* SilentParseFunction(SilentNamespace &scope);
    SilentNamespace* SilentParseNamespace(SilentNamespace &scope);
    SilentParserInfo* SilentParse(std::vector<Silent::SilentToken> tokens);
}