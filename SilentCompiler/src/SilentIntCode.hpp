#pragma once
#include "SilentParser.hpp"
namespace Silent
{
    enum class SilentIntToken
    {

    };

    struct SilentIntCode
    {
        std::string code;
        
    };

    SilentIntCode* SilentTransform(SilentParserInfo* parsedCode);
}