#pragma once
#include "SilentParser.hpp"
namespace Silent
{
    std::vector<std::string> SilentTransform(std::vector<SilentNode*>* nodes);
    std::vector<std::string> SilentGenAssembly(std::vector<std::string> code);
    void SilentPrintTree(SilentOperand* operand);    
}
