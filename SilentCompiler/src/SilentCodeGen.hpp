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

    //std::vector<SilentIntCode>* SilentTransform(std::vector<SilentNode*>& nodes);
    //std::vector<std::string> SilentGenAssembly(std::vector<std::string> code); 
}