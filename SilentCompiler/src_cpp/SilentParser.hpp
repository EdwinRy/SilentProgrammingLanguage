#pragma once
#include <vector>
#include "SilentTokenizer.hpp"
namespace SilentParser
{
    enum silentValueType
    {
        silentNumericalValue,
        silentTextValue,
        silentNullValue
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

    struct silentExpression
    {
        std::vector<std::string> expression;
    };

    struct silentValue
    {
        silentValueType valueType; 
        silentExpression value;
    };

    struct silentVariable
    {
        silentValue value;
        silentDataType dataType;
        std::string name;
        unsigned int size;
    };

    struct silentStructure
    {
        std::string name;
        std::vector<silentVariable> variables;
        unsigned int size;
        unsigned int items;
    };

    struct silentWhileLoop
    {

    };

    struct silentFunction
    {
        std::vector<std::string> expressions;
        std::vector<silentVariable> arguments;
        std::vector<silentVariable> variables;
        std::string name;
        std::vector<silentValue> returnValues;
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