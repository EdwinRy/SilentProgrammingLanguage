#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    enum class SilentTokenType
    {
        Unrecognised,
        Identifier,
        Number,
        String,
        OpenParam,
        CloseParam,
        OpenScope,
        CloseScope,
        Multiply,
        Divide,
        Add,
        Subtract,
        Semicolon,
        Primitive,
        Comma,
        FullStop,
        Assign,
        Function,
        Struct,
        Return,
        Namespace,
        Class,
        Method
    };

    typedef struct SilentToken
    {
        SilentTokenType type;
        std::string value;
        unsigned long long line;
    }SilentToken;

    std::vector<SilentToken>* SilentTokenize(std::string source);

}