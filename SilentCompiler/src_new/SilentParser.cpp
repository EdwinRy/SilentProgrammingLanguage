#include "SilentParser.hpp"
using namespace Silent;
typedef unsigned long long uint64;

std::vector<SilentNode>* nodes;

bool checkAccessibleIdentifier(std::string name)
{
    for(uint64 i = 0; i < nodes->size(); i++)
    {
        if((*nodes)[i].name == name)
        {
            return true;
        }
    }
    return false;
}

SilentNode* findStruct(std::string name)
{
    for(uint64 i = 0; i < nodes->size(); i++)
    {
        if((*nodes)[i].type == SilentNodeType::structure)
        {
            if((*nodes)[i].name == name)
            {
                return &(*nodes)[i];
            }
        }
    }
    return NULL;
}

SilentDataType getType(std::string name)
{
    if(name == "int8")
    {
        return SilentDataType::int8;
    }
    else if(name == "uint8")
    {
        return SilentDataType::uint8;
    }
    else if(name == "int16")
    {
        return SilentDataType::int16;
    }
    else if(name == "uint16")
    {
        return SilentDataType::uint16;
    }
    else if(name == "int32")
    {
        return SilentDataType::int32;
    }
    else if(name == "uint64")
    {
        return SilentDataType::uint64;
    }
    else if(name == "float32")
    {
        return SilentDataType::float32;
    }
    else if(name == "float64")
    {
        return SilentDataType::float64;
    }
    else if(name == "string")
    {
        return SilentDataType::string;
    }
    else if(name == "pointer")
    {
        return SilentDataType::pointer;
    }
    else if(name == "void")
    {
        return SilentDataType::null;
    }
    else
    {
        SilentNode* s = findStruct(name);
        if(s != NULL)
        {
            return SilentDataType::structType;
        }
    }
    return SilentDataType::undefined;
}

void SilentParseVar(
    std::vector<Silent::SilentToken> tokens, uint64* i,
    SilentDataType type, bool init
)
{
    SilentNode node;
    node.variable = new SilentVariable();
    node.type = SilentNodeType::variable;
    node.variable->type = type;

    if(type == SilentDataType::structType)
    {
        node.variable->typePtr = findStruct(tokens[*i].value);
    }
    *i += 1;
    if(!checkAccessibleIdentifier(tokens[*i].value))node.name=tokens[*i].value;
    else{
        printf("Error on line %i:\n",tokens[*i].line);
        printf("Identifier %s already in use\n",tokens[*i].value.data());
    }
    *i += 1;
    if(init)
    {
        if(tokens[*i].value != ";"){
            printf("Error on line %i:\n",tokens[*i].line);
            printf("Expected \";\" at the end of declaration\n");
        }
        nodes->push_back(node);
        printf("declared variable %s\n",node.name.data());
        return;
    }
    else
    {
        if(tokens[*i].type != SilentTokenType::expressionSymbol)
        {
            printf("Error on line %i:\n",tokens[*i].line);
            printf("Expected an expression for the variable declaration\n");
        }
    }
}

std::vector<SilentNode>* Silent::SilentParse(
    std::vector<Silent::SilentToken> tokens
)
{
    std::vector<SilentNode>* out = new std::vector<SilentNode>();
    nodes = out;
    //Look for function, struct and variable declarations
    for(uint64 i = 0; i < tokens.size(); i++)
    {
        if(tokens[i].type == SilentTokenType::keyword)
        {
            if(tokens[i].value == "struct")
            {

            }
            else if(tokens[i].value == "func")
            {

            }
            else
            {
                printf("Error on line %i:\n",tokens[i].line);
                printf("Unexpected token in the global scope");
            }
        }
        else
        {
            SilentDataType type = getType(tokens[i].value);
            if(type != SilentDataType::undefined)
            {
                SilentParseVar(tokens,&i,type,true);
            }
            else
            {
                printf("Error on line %i:\n",tokens[i].line);
                printf("Type \"%s\" was not defined",tokens[i].value.data());
                exit(-1);
            }
        }
    }
    return out;
}