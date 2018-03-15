#pragma once
#include <iostream>
#include <vector>
namespace SilentTokenizer
{
    //Type of the token
    typedef enum silentTokenType
    {
        silentUnrecognisedToken,    //For unknown values
        silentIdentifierToken,      //For function names etc
        silentMathsOperatorToken,   //+, -, * etc...
        silentOpenBracketToken,     //All opening brackets
        silentClosingBracketToken,  //All closing brackets
        silentComparisonToken,      //>, <, <= etc
        silentStructureToken,       //For structure declarations
        silentAccessToken,          //private, public etc
        silentNumberToken,          //for the actual numerical data like 2.5
        silentStringToken,          //for the actual text data
        silentTypeToken,            //for types such as int
        silentOtherToken            //mostly ".", "," etc

    }silentTokenType;

    typedef struct silentToken
    {
        silentTokenType type;
        std::string value;
        unsigned int currentLine;
    }silentToken;

    std::vector<silentToken>* silentTokenize(std::string source);
}