#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    enum class SilentTokenType
    {
        Unrecognised,
        Identifier,
        ExpressionSymbol,
        Symbol,
        Function,
        Struct,
        Return,
        Primitive,
        Number,
        StringValue,
        OpenScope,
        CloseScope,
        OpenParam,
        CloseParam
    };

    typedef struct SilentToken
    {
        SilentTokenType type;
        std::string value;
        unsigned long long line;
    }SilentToken;

    std::vector<SilentToken>* SilentTokenize(std::string source);

}