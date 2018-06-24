#pragma once
#include <iostream>
#include <vector>
#include "SilentTokenizer.hpp"
namespace Silent
{
    typedef struct SilentNode SilentNode;
    enum class SilentNodeType
    {
        variable,
        structure,
        function,
        scope
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

    typedef struct SilentVariable SilentVariable;
    typedef struct SilentStructure SilentStructure;
    typedef struct SilentFunction SilentFunction;
    typedef struct SilentOperand SilentOperand;
    typedef struct SilentNode
    {
        SilentNodeType type;
        std::string name;
        union
        {
            SilentVariable* variable;
            SilentStructure* structure;
            SilentFunction* function;
        };
    }SilentNode;

    typedef struct SilentVariable
    {
        unsigned long long size;
        SilentDataType type;
        SilentNode* typePtr;
        SilentOperand* expresion;
        bool initialised;
        unsigned long long localPos;
    }SilentVariable;

    typedef struct SilentStructure
    {
        unsigned long long size;
        std::vector<SilentNode*> variables;
    }SilentStructure;

    typedef struct SilentFunction
    {
        std::vector<SilentNode*> parameters;
        bool initialised;
        unsigned long long returnTypePtr;
        unsigned long long returnSize;
        SilentDataType returnType;
    }SilentFunction;

    typedef struct SilentOperand
    {
        SilentOperandType type;
        SilentOperand *left, *right;
        union
        {
            SilentNode* nodeData;
            SilentToken* tokenData;
            SilentVariable* varData;
        };
    }SilentOperant;


    void SilentParseExpression(
        std::vector<SilentNode*> *scope,
        std::vector<Silent::SilentToken> tokens,
        unsigned long long* i,
        SilentOperand* operand
    );

    SilentNode* SilentParseVar(
        std::vector<SilentNode*> *scope,
        std::string type,
        bool init,
        bool expectEnd
    );

    SilentNode* SilentParseStruct(std::vector<SilentNode*> *scope);

    void SilentParseParameters(
        SilentFunction* function,
        std::vector<SilentNode*> *scope
    );

    SilentNode* SilentParseScope();

    SilentNode* SilentParseFunction(std::vector<SilentNode*> *scope);

    std::vector<SilentNode*>* SilentParse(
        std::vector<Silent::SilentToken> tokens
    );
}