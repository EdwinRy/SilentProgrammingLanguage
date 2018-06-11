#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    enum class SilentTokenType
    {
        unrecognised,
        identifier,
        expressionSymbol,
        symbol,
        keyword,
        numericalValue,
        stringValue,
    };

    typedef struct SilentToken
    {
        SilentTokenType type;
        std::string value;
        unsigned long long line;
    }SilentToken;

    std::vector<SilentToken>* SilentTokenize(std::string source);

}