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

    typedef struct SilentVariable
    {
        SilentDataType type;
        SilentNode* typePtr;
        bool initialised;
        unsigned long long localPos;
        unsigned long long size;
    }SilentVariable;

    typedef struct SilentStructure
    {
        bool initialised;
        std::vector<SilentVariable> variables;
        unsigned long long size;
    }SilentStructure;

    typedef struct SilentFunction
    {
        bool initialised;
    }SilentFunction;

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

    std::vector<SilentNode>* SilentParse(
        std::vector<Silent::SilentToken> tokens
    );
}