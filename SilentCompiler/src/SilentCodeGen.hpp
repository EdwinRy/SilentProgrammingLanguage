#pragma once
#include "SilentParser.hpp"
#include <vector>
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
    std::string SilentGenerateAssembly(std::string code);
}