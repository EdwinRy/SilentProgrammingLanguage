#include "SilentParser.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<SilentNode> NodeList;

#define DEBUG 1

NodeList accessibleScope;
std::vector<uint64> scopeBreaks;
uint64 currentPos;

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
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout <<"Identifier "<<tokens[*i].value.data()<<" already in use\n";
        exit(-1);
    }
    else node->name=tokens[*i].value;
    *i += 1;

    if(init)
    {
        if(tokens[*i].value != ";"){
            std::cout << "Error on line "<<tokens[*i].line <<":\n";
            std::cout << "Expected \";\" at the end of declaration\n";
            exit(-1);
        }
#if DEBUG
        std::cout << "declared variable " << node->name.data() << "\n";
#endif
        return node;
    }
    else
    {
        if(tokens[*i].type != SilentTokenType::ExpressionSymbol)
        {
            std::cout << "Error on line "<<tokens[*i].line <<":\n";
            std::cout<<"Expected an expression for the variable declaration\n";
            exit(-1);
        }
        //Add Expression parse
    }
}

SilentNode* SilentParseStruct(NodeList* scope, TokenList tokens, uint64* i)
{
    SilentNode* node = new SilentNode();
    node->structure = new SilentStructure();
    node->type = SilentNodeType::structure;
    NodeList* localScope = &(node->structure->variables);
    *i += 1;

    if(getType(scope, tokens[*i].value) != SilentDataType::undefined)
    {
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout<<"Identifier "<<tokens[*i].value.data()<<" already in use \n";
        exit(-1);
    }
    else node->name=tokens[*i].value;
    *i += 1;

    if(tokens[*i].value != "{")
    {
        std::cout << "Error on line "<< tokens[*i].line <<":\n";
        std::cout << "Expected struct declaration\n";
        exit(-1);
    }
    *i += 1;
    for(;tokens[*i].value != "}";*i+=1)
    {
        SilentDataType type = getType(scope, tokens[*i].value);
        if(type != SilentDataType::undefined)
        {
            SilentNode* node = SilentParseVar(localScope,tokens,i,type,true);
            localScope->push_back(*node);
            delete node;
        }
        else
        {
            std::cout << "Error on line "<<tokens[*i].line <<":\n";
            std::cout<<"Use of non-existing type "<<tokens[*i].value.data()<<"\n";
            exit(-1);
        }
    }

#if DEBUG
    std::cout
    << "structure " << node->name.data() << " declared with variables:\n";

    for(uint64 i = 0; i < localScope->size(); i++)
    {
        std::cout << i << ":" << (*localScope)[i].name.data() << "\n";
    }
#endif

    return node;
}

NodeList* Silent::SilentParse(TokenList tokens)
{
    NodeList* globalScope = new NodeList();
    //Look for function, struct and variable declarations
    for(uint64 i = 0; i < tokens.size(); i++)
    {
        if(tokens[i].type == SilentTokenType::Struct)
        {
            SilentNode* node = SilentParseStruct(globalScope, tokens,&i);
            globalScope->push_back(*node);
            delete node;
        }

        else if(tokens[i].type == SilentTokenType::Function)
        {

        }

        else if(tokens[i].type == SilentTokenType::Primitive)
        {
            SilentDataType type = getType(globalScope, tokens[i].value);
            SilentNode* node = SilentParseVar(globalScope,tokens,&i,type,true);
            globalScope->push_back(*node);
            delete node;
        }

        else if(tokens[i].type == SilentTokenType::Identifier)
        {
            SilentDataType type = getType(globalScope, tokens[i].value);
            if(type == SilentDataType::structType)
            {
                SilentNode* node = SilentParseVar(globalScope,tokens,&i,type,true);
                globalScope->push_back(*node);
                delete node;
            }
            else
            {
                std::cout << "Error on line "<<tokens[i].line <<":\n";
                std::cout
                << "Use of non-existant type "
                << tokens[i].value.data() <<"\n";
                exit(-1);
            }
        }

        else
        {
            std::cout << "Error on line "<<tokens[i].line <<":\n";
            std::cout
            << "Unexpected token in the global scope "
            << tokens[i].value.data() <<"\n";
            exit(-1);
        }
    }
    return globalScope;
}