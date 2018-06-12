#include "SilentParser.hpp"
using namespace Silent;
typedef unsigned long long uint64;

std::vector<SilentNode> nodes;

bool checkAccessibleIdentifier(std::string name)
{
    for(uint64 i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].name == name)
        {
            return true;
        }
    }
    return false;
}

SilentNode* findStruct(std::string name)
{
    for(uint64 i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].type == SilentNodeType::structure)
        {
            if(nodes[i].name == name)
            {
                return &nodes[i];
            }
        }
    }
    return NULL;
}

SilentDataType getType(std::string name)
{

}

void SilentParseVar(
    std::vector<Silent::SilentToken> tokens, uint64* i, bool init
)
{
    *i += 1;
    SilentNode node;
    node.variable = new SilentVariable();
    node.type = SilentNodeType::variable;

    if(tokens[*i].type != SilentTokenType::primitive)
    {
        node.variable->type = SilentDataType::structType;
        node.variable->typePtr = findStruct(tokens[*i].value);
    }

    //If its exclusively initialisation
    if(init)
    {

    }
}

std::vector<std::string>* SilentParse(std::vector<Silent::SilentToken> tokens)
{
    std::vector<std::string>* out = new std::vector<std::string>();
    //Look for function, struct and variable declarations
    for(uint64 i = 0; i < tokens.size(); i++)
    {
        if(tokens[i].type == SilentTokenType::keyword)
        {
            if(tokens[i].value == "var")
            {
                SilentParseVar(tokens, &i, true);
            }
            else if(tokens[i].value == "struct")
            {

            }
            else if(tokens[i].value == "func")
            {

            }
        }
    }

    return out;
}