#pragma once
#include <iostream>
#include <vector>
namespace SilentTokenizer
{
    typedef enum silentTokenType
    {
        silentUnrecognisedToken,    //For unknown values
        silentIdentifierToken,      //For function names etc
        silentMathsOperatorToken,   //+, -, * etc...
        silentExpressionToken,      // =, += etc
        silentOpenBracketToken,     //All opening brackets
        silentClosingBracketToken,  //All closing brackets
        silentComparisonToken,      //>, <, <= etc
        silentStructureToken,       //For structure declarations
        silentAccessToken,          //private, public etc
        silentNumberToken,            //for the actual data like 2.5
        silentStringToken,
        silentTypeToken,            //for types such as int
        silentOtherToken            //mostly ".", "," etc

    }silentTokenType;

    typedef struct silentToken
    {
        silentTokenType type;
        std::string value;
        int currentLine;
    }silentToken;

    std::vector<silentToken>* silentTokenize(std::string source);
}