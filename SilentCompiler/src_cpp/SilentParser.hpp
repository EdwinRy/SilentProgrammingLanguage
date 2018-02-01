#pragma once
#include <vector>
#include "SilentTokenizer.hpp"
namespace SilentParser
{
    enum silentValueType
    {
        silentNumericalValue,
        silentTextValue
    };

    enum silentDataType
    {
        silentIntType,
        silentLongType,
        silentFloatType,
        silentDoubleType,
        silentStringType
    };
    //using namespace SilentTokenizer;

    struct silentValue
    {
        silentValueType valueType; 
        std::string value;
    };

    struct silentVariable
    {
        silentValue value;
        silentDataType dataType;
        std::string name;
    };

    struct silentExpression
    {

    };

    struct silentStructure
    {
        std::string name;
        std::vector<silentVariable> variables;
        unsigned int size;
        unsigned int items;
    };

    struct silentFunction
    {
        std::vector<silentExpression> expressions;
        std::string name;
        silentValue returnType;
    };
    
    struct silentProgram
    {
        std::vector<silentFunction> functions;
        std::vector<silentStructure> structures;
    };

    silentProgram silentParseProgram(
        std::vector<SilentTokenizer::silentToken> tokens);
}