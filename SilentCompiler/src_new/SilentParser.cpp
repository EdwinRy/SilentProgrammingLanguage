#include "SilentParser.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<SilentNode> NodeList;

NodeList* nodes;

bool checkAccessibleIdentifier(NodeList* scope, std::string name)
{
    for(uint64 i = 0; i < scope->size(); i++)
    {
        if((*scope)[i].name == name)
        {
            return true;
        }
    }
    return false;
}

SilentNode* findStruct(NodeList* scope, std::string name)
{
    for(uint64 i = 0; i < scope->size(); i++)
    {
        if((*scope)[i].type == SilentNodeType::structure)
        {
            if((*scope)[i].name == name)
            {
                return &(*scope)[i];
            }
        }
    }
    return NULL;
}

SilentDataType getType(NodeList* scope, std::string name)
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
        SilentNode* s = findStruct(scope,name);
        if(s != NULL)
        {
            return SilentDataType::structType;
        }
    }
    return SilentDataType::undefined;
}

SilentNode* SilentParseVar(
    NodeList* scope,
    TokenList tokens,
    uint64* i,
    SilentDataType type,
    bool init
)
{
    SilentNode* node = new SilentNode();
    node->variable = new SilentVariable();
    node->type = SilentNodeType::variable;
    node->variable->type = type;

    if(type == SilentDataType::structType)
    {
        node->variable->typePtr = findStruct(scope, tokens[*i].value);
    }
    *i += 1;
    if(checkAccessibleIdentifier(scope, tokens[*i].value))
    {
        printf("Error on line %i:\n",tokens[*i].line);
        printf("Identifier %s already in use\n",tokens[*i].value.data());
    }
    else node->name=tokens[*i].value;
    *i += 1;

    if(init)
    {
        if(tokens[*i].value != ";"){
            printf("Error on line %i:\n",tokens[*i].line);
            printf("Expected \";\" at the end of declaration\n");
        }
        printf("declared variable %s\n",node->name.data());
        return node;
    }
    else
    {
        if(tokens[*i].type != SilentTokenType::expressionSymbol)
        {
            printf("Error on line %i:\n",tokens[*i].line);
            printf("Expected an expression for the variable declaration\n");
        }
        //Add Expression parse
    }
}

SilentNode* SilentParseStruct(NodeList* scope, TokenList tokens, uint64* i)
{
    SilentNode* node = new SilentNode();
    node->structure = new SilentStructure();
    node->type = SilentNodeType::structure;
    *i += 1;
    if(checkAccessibleIdentifier(scope, tokens[*i].value))
    {
        printf("Error on line %i:\n",tokens[*i].line);
        printf("Identifier %s already in use\n",tokens[*i].value.data());
    }
    else node->name=tokens[*i].value;

    *i += 1;
    if(tokens[*i].value != "{")
    {
        printf("Error on line %i:\n",tokens[*i].line);
        printf("Expected struct declaration\n");
    }
    *i += 1;

    for(;tokens[*i].value != "}";*i+=1)
    {
        SilentDataType type = getType(scope, tokens[*i].value);
        if(type != SilentDataType::undefined)
        {
            NodeList* list = &node->structure->variables;
            SilentNode* node = SilentParseVar(list,tokens,i,type,true);
            node->structure->variables.push_back(*node);
            //delete node;
        }
        else
        {
            printf("Error on line %i:\n",tokens[*i].line);
            printf("Use of non-existing type %s\n",tokens[*i].value.data());
        }
    }

    NodeList* list = &node->structure->variables;
    printf("structure %s declared with variables:\n",node->name.data());
    for(uint64 i = 0; i < list->size(); i++)
    {
        printf("%i:\n",i);
    }
}

NodeList* Silent::SilentParse(TokenList tokens)
{
    NodeList* out = new NodeList();
    nodes = out;
    //Look for function, struct and variable declarations
    for(uint64 i = 0; i < tokens.size(); i++)
    {
        switch(tokens[i].type)
        {
            case SilentTokenType::Struct:
                SilentNode* node = SilentParseStruct(nodes, tokens,&i);
                nodes->push_back(*node);
                delete node;
            break;
            case SilentTokenType::Function:

            break;
            case SilentTokenType::Identifier:
                SilentDataType type = getType(nodes, tokens[i].value);
                if(type != SilentDataType::undefined)
                {
                    SilentNode* node = SilentParseVar(nodes,tokens,&i,type,true);
                    nodes->push_back(*node);
                    delete node;
                }
                else
                {
                    printf("Error on line %i:\n",tokens[i].line);
                    printf("Type \"%s\" was not defined",tokens[i].value.data());
                    exit(-1);
                }
            break;
            default:
                printf("Error on line %i:\n",tokens[i].line);
                printf("Unexpected token in the global scope");
            break;
        }
    }
    return out;
}