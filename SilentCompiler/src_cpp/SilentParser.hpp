#pragma once
#include <vector>
#include "SilentTokenizer.hpp"
namespace SilentParser
{
    using namespace SilentTokenizer;
    typedef struct silentValue
    {

    }silentValue;
    typedef struct silentVariable
    {

    }silentVariable;
    typedef struct silentExpression
    {

    }silentExpression;
    typedef struct silentStructure
    {

    }silentStructure;
    typedef struct silentFunction
    {

    }silentFunction;
    typedef struct silentProgram
    {

    }silentProgram;

    silentProgram* silentParseProgram(std::vector<silentToken> tokens);
}