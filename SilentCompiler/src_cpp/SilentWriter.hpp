#pragma once
#include <iostream>
#include <vector>
#include "SilentParser.hpp"
namespace SilentCodeGenerator
{
    std::string compileBytecode(SilentParser::silentProgram* program);
    std::vector<std::string> compileAssembly(SilentParser::silentProgram* program);
    std::vector<std::string> compileLibrary(SilentParser::silentProgram* program);
}