#pragma once
#include "SilentParser.hpp"
namespace Silent
{
    enum class SilentIntToken
    {

    };

    struct SilentIntCode
    {
        SilentIntToken type;
        union
        {
            std::string strData;
            void* ptrData;
            char* charData;
        };
    };

    std::vector<SilentIntCode>* SilentTransform(std::vector<SilentNode*>* nodes);
    std::vector<std::string> SilentGenAssembly(std::vector<std::string> code);
    void SilentPrintTree(SilentOperand* operand);    
}
