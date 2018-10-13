#pragma once
#include "SilentParser.hpp"
#include <vector>
namespace Silent
{
    std::string SilentCompileAST(SilentParserInfo& info);
    std::string SilentGenerateAssembly(std::string code);
    std::string SilentCompileIntCode(std::string intCode);
    std::string SilentGenerateIntCode(SilentParserInfo* parsedCode);
}