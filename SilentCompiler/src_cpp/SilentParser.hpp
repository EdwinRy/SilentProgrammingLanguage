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
        silentStringType,
        silentStructType,
        silentNullType
    };
    
    enum silentExpressionType
    {
        silentAddition,
        silentSubtraction,
        silentMultiplication,
        silentDivision,
        silentAssignment,
        silentValueHolder,
        silentValueReference
    };

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
        unsigned int size;
    };

    struct silentExpression
    {
        std::vector<std::string> expression;
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
        std::vector<silentVariable> arguments;
        std::string name;
        silentValue returnValue;
        silentDataType returnType;
    };
    
    struct silentProgram
    {
        std::vector<silentFunction> functions;
        std::vector<silentStructure> structures;
        std::vector<silentVariable> globals;
    };

    silentProgram *silentParseProgram(
        std::vector<SilentTokenizer::silentToken> tokens);
}