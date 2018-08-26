#pragma once
#include <iostream>
#include <vector>
#include "SilentTokenizer.hpp"
namespace Silent
{
    enum class SilentStatementType
    {
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
        Value,
        Identifier
    };

    enum class SilentExpressionType
    {
        arithmetic,
        logical
    };

    typedef struct SilentVariable SilentVariable;
    typedef struct SilentStructure SilentStructure;
    typedef struct SilentFunction SilentFunction;
    typedef struct SilentOperand SilentOperand;
    typedef struct SilentLocalScope SilentLocalScope;

    typedef struct SilentVariable
    {
        unsigned long long size;
        unsigned long long localPos;
        SilentOperand* expresion;
        SilentDataType type;
        bool initialised;
    }SilentVariable;

    typedef struct SilentStructure
    {
        std::vector<SilentVariable*> variables;
        std::string name;
        unsigned long long size;
        bool initialised;
    }SilentStructure;

    typedef struct SilentLocalScope
    {
        std::vector<SilentVariable*> variables;
        std::vector<SilentStatement*> statements;
        bool scopeParent;
        union
        {
            SilentLocalScope* scopeParent;
            SilentNamespace* namespaceParent;
        };
    }SilentLocalScope;

    typedef struct SilentFunction
    {
        std::vector<SilentVariable*> parameters;
        SilentLocalScope* scope;
        std::string name;
        unsigned long long returnTypePtr;
        unsigned long long returnSize;
        SilentDataType returnType;
        bool initialised;
    }SilentFunction;

    typedef struct SilentExpression
    {
        SilentOperand* operand;
        SilentExpressionType type;
        SilentDataType returnType;
    };

    typedef struct SilentStatement
    {
        SilentStatementType type;
        
    }SilentStatement;

    typedef struct SilentOperand
    {
        SilentOperandType type;
        SilentOperand *left, *right;
        union
        {
            SilentToken* token;
            SilentVariable* variable;
            SilentExpression* expression;
        };
    }SilentOperant;

    typedef struct SilentNamespace
    {
        SilentNamespace* parent;
        std::vector<SilentNamespace*> namespaces;
        std::vector<SilentFunction*> functions;
        std::vector<SilentStructure*> types;
        std::vector<SilentVariable*> globals;
        std::string name;
    }SilentNamespace;

    typedef struct SilentParserInfo
    {
        std::vector<SilentNamespace*> namespaces;
    }SilentParserInfo;


    SilentExpression* SilentParseExpression(
        SilentLocalScope &scope,
        SilentOperand* operand
    );

    SilentStatement* SilentParseStatement(SilentLocalScope &scope);

    SilentVariable* SilentParseVar(
        SilentLocalScope &scope,
        std::string type,
        bool init,
        bool expectEnd
    );

    SilentStructure* SilentParseStruct(SilentNamespace &scope);
    SilentStructure* SilentParseParameters(SilentFunction &function);
    SilentLocalScope* SilentParseLocalScope(SilentNamespace &scope);
    SilentLocalScope* SilentParseFunctionScope();
    SilentFunction* SilentParseFunction(SilentNamespace &scope);
    SilentNamespace* SilentParseNamespace(SilentNamespace &scope);
    SilentParserInfo* SilentParse(std::vector<Silent::SilentToken> tokens);
}