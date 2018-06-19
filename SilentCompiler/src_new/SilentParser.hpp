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
        function
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

    typedef struct SilentVariable SilentVariable;
    typedef struct SilentStructure SilentStructure;
    typedef struct SilentFunction SilentFunction;
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

    typedef struct SilentOperant
    {

    }SilentOperant;

    std::vector<SilentNode*>* SilentParse(
        std::vector<Silent::SilentToken> tokens
    );
}