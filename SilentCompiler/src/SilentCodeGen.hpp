#pragma once
#include "SilentParser.hpp"
#include <vector>
namespace Silent
{
    std::string SilentGenerateAssembly(std::string code);
    std::string SilentCompileIntCode(std::string intCode);
}