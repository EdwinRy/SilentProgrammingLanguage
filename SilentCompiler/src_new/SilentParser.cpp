#include "SilentParser.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<Silent::SilentToken*> TokenPtrList;
//typedef std::vector<SilentNode> NodeList;
typedef std::vector<SilentNode*> NodePtrList;

#define DEBUG 1


NodePtrList accessibleScope;


SilentNode* getNode(NodePtrList* scope, std::string name, SilentNodeType type)
{
    for(uint64 i = 0; i < (*scope).size(); i++)
    {
        if((*scope)[i]->type == type)
        {
            if((*scope)[i]->name == name) return (*scope)[i];
        }
    }
    return NULL;
}

SilentNode* getAccessibleNode(std::string name, SilentNodeType type)
{
    //Scan from the end to get the closest variable
    for(uint64 i = accessibleScope.size()-1; i >= 0; i--)
    {
        if(accessibleScope[i]->type == type)
        {
            if(accessibleScope[i]->name == name) return accessibleScope[i];
        }
    }
    return NULL;
}

SilentDataType getType(NodePtrList* scope, std::string name)
{
    if(name == "int8") return SilentDataType::int8;
    else if(name == "uint8") return SilentDataType::uint8;
    else if(name == "int16") return SilentDataType::int16;
    else if(name == "uint16") return SilentDataType::uint16;
    else if(name == "int32") return SilentDataType::int32;
    else if(name == "uint32") return SilentDataType::uint32;
    else if(name == "int64") return SilentDataType::int64;
    else if(name == "uint64") return SilentDataType::uint64;
    else if(name == "float32") return SilentDataType::float32;
    else if(name == "float64") return SilentDataType::float64;
    else if(name == "string") return SilentDataType::string;
    else if(name == "pointer") return SilentDataType::pointer;
    else if(name == "void") return SilentDataType::null;
    else
    {
        if(getNode(scope,name,SilentNodeType::structure) != NULL) 
        {
            return SilentDataType::structType;}
    }
    return SilentDataType::undefined;
}

uint64 getTypeSize(NodePtrList* scope, std::string name)
{
    if(name == "int8") return 1;
    else if(name == "uint8") return 1;
    else if(name == "int16") return 2;
    else if(name == "uint16") return 2;
    else if(name == "int32") return 4;
    else if(name == "uint32") return 4;
    else if(name == "int64") return 8;
    else if(name == "uint64") return 8;
    else if(name == "float32") return 4;
    else if(name == "float64") return 8;
    else if(name == "string") return 8;
    else if(name == "pointer") return 8;
    else if(name == "void") return 0;
    else
    {
        SilentNode* node = getNode(scope,name,SilentNodeType::structure);
        if(node != NULL) return node->structure->size;
    }
    return -1;
}

uint64 getLocalPos(NodePtrList* scope)
{
    uint64 scopeSize = (*scope).size();
    uint64 lastPos = 1;
    if(scopeSize == 0) return 0;
    else
    {
        while((*scope)[scopeSize-lastPos]->type == SilentNodeType::structure)
        {
            lastPos+=1;
        }
        SilentVariable* var = (*scope)[scopeSize-lastPos]->variable;
        return var->localPos + var->size;
    }
}

std::string getNextToken()
{
    
}
void Silent::SilentParseExpression(
    NodePtrList* scope, TokenList tokens, uint64* i, SilentOperand* operand
)
{
    *i += 1;

    if(operand->leftUsed)
    {

    }
    else if(tokens[*i].value == "=")
    {
        SilentOperand* op = new SilentOperand();
        op->type = SilentOperandType::Assign;
        op->left = operand;
        SilentParseExpression(scope,tokens,i,op);
    }
}

SilentNode* Silent::SilentParseVar(
    NodePtrList* scope,
    TokenList tokens,
    uint64* i,
    std::string type,
    bool init,
    bool expectEnd
)
{
    SilentNode* node = new SilentNode();
    node->variable = new SilentVariable();
    node->type = SilentNodeType::variable;
    node->variable->type = getType(scope,type);
    node->variable->size = getTypeSize(scope,type);

    if(node->variable->type == SilentDataType::undefined)
    {
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout <<"Type "<<tokens[*i].value.data()<<" undefined\n";
    }


    else if(node->variable->type == SilentDataType::structType)
    {
        node->variable->typePtr = 
        getNode(scope, tokens[*i].value,SilentNodeType::structure);
        
    }
    node->variable->localPos = getLocalPos(scope);
    *i += 1;

    if(getNode(scope,tokens[*i].value,SilentNodeType::variable) != NULL)
    {
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout <<"Identifier "<<tokens[*i].value.data()<<" already in use\n";
        exit(-1);
    }
    else node->name = tokens[*i].value;
    *i += 1;

    if(init)
    {
        if(expectEnd)
        {
            if(tokens[*i].value != ";"){
                std::cout << "Error on line "<<tokens[*i].line <<":\n";
                std::cout << "Expected \";\" at the end of declaration\n";
                exit(-1);
            }
        }
#if DEBUG
        std::cout << "Declared variable " << node->name.data() << "\n"
        << "of size: " << node->variable->size << "\n"
        << "at local position " << node->variable->localPos << "\n\n";
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

SilentNode* Silent::SilentParseStruct(NodePtrList* scope, TokenList tokens, uint64* i)
{
    SilentNode* node = new SilentNode();
    node->structure = new SilentStructure();
    node->type = SilentNodeType::structure;
    NodePtrList* localScope = &(node->structure->variables);
    *i += 1;

    if(getType(scope, tokens[*i].value) != SilentDataType::undefined)
    {
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout<<"Identifier "<<tokens[*i].value.data()<<" already in use \n";
        exit(-1);
    }
    else node->name = tokens[*i].value;
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
        if(getType(scope,tokens[*i].value) != SilentDataType::undefined)
        {
            SilentNode* var = SilentParseVar(
                localScope, tokens, i, tokens[*i].value, true, true
            );
            localScope->push_back(var);
            node->structure->size += var->variable->size;
#if DEBUG
            std::cout << "struct size " << node->structure->size << "\n";
#endif
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
    << "structure " << node->name.data() << " of size: " << node->structure->size
    << " declared with variables:\n";

    for(uint64 i = 0; i < localScope->size(); i++)
    {
        std::cout << i << ":" << (*localScope)[i]->name.data() << "\n";
    }
    std::cout << "\n";
#endif
    return node;
}

void Silent::SilentParseParameters(
    SilentFunction* function,
    NodePtrList* scope,
    TokenList tokens,
    uint64* i
)
{
    //for(; tokens[*i].type != SilentTokenType::CloseParam; *i+=1)
    while(true)
    {
        function->parameters.push_back(
            SilentParseVar(
                &function->parameters,tokens,i,
                tokens[*i].value,true,false
            )
        );
        if(tokens[*i].value == ",")
        {
            *i += 1;
            continue;
        }
        else if(tokens[*i].type == SilentTokenType::CloseParam)
        {
            *i+=1;
            break;
        }
        else
        {
            std::cout << "Error on line "<<tokens[*i].line <<":\n";
            std::cout << "Invalid token when parsing parameters " 
            << tokens[*i].value.data() << "\n";
        }
    }
}

SilentNode* Silent::SilentParseFunction(NodePtrList* scope, TokenList tokens, uint64* i)
{
    SilentNode* node = new SilentNode();
    node->function = new SilentFunction();
    node->type = SilentNodeType::function;
    *i += 1;

    node->function->returnType = getType(scope,tokens[*i].value);
    if(node->function->returnType == SilentDataType::undefined)
    {
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout <<"Type "<<tokens[*i].value.data()<<" undefined\n";
        exit(-1);
    }

    *i += 1;
    if(getNode(scope, tokens[*i].value, SilentNodeType::function) != NULL)
    {
        std::cout << "Error on line "<<tokens[*i].line <<":\n";
        std::cout <<"Identifier "<<tokens[*i].value.data()<<" already in use\n";
        exit(-1);
    }
    else node->name = tokens[*i].value;
    *i += 1;

    if(tokens[*i].type != SilentTokenType::OpenParam)
    {
        std::cout << "Error on line "<< tokens[*i].line <<":\n";
        std::cout << "Expected \"(\" for parameter declaration\n";
        exit(-1);
    }

    *i += 1;
    SilentParseParameters(node->function,scope,tokens,i);

    if(tokens[*i].type != SilentTokenType::OpenScope)
    {
        node->function->initialised = false;
        if(tokens[*i].value != ";")
        {
            std::cout << "Error on line "<< tokens[*i].line <<":\n";
            std::cout << "Expected \";\" at the end of uninitialised function\n";
        }
    }
    else
    {
        node->function->initialised = true;
    }

#if DEBUG
    std::cout << "Declared function " << node->name.data() << "\n";
#endif

    return node;
}

NodePtrList* Silent::SilentParse(TokenList tokens)
{
    NodePtrList* globalScope = new NodePtrList();
    //Look for function, struct and variable declarations
    for(uint64 i = 0; i < tokens.size(); i++)
    {
        if(tokens[i].type == SilentTokenType::Struct)
        {
            SilentNode* node = SilentParseStruct(globalScope, tokens, &i);
            globalScope->push_back(node);
        }

        else if(tokens[i].type == SilentTokenType::Function)
        {
            SilentNode* node = SilentParseFunction(globalScope,tokens,&i);
            globalScope->push_back(node);
        }

        else if(tokens[i].type == SilentTokenType::Primitive)
        {
            SilentNode* node = SilentParseVar(
                globalScope,tokens,&i,tokens[i].value,true,true
            );
            globalScope->push_back(node);
        }

        else if(tokens[i].type == SilentTokenType::Identifier)
        {
            SilentDataType type = getType(globalScope, tokens[i].value);
            if(type == SilentDataType::structType)
            {
                SilentNode* node = SilentParseVar(
                    globalScope,tokens,&i,tokens[i].value,true,true
                );
                globalScope->push_back(node);
                //accessibleScope.push_back(node);
            }
            else
            {
                std::cout << "Error on line "<<tokens[i].line <<":\n";
                std::cout
                << "Use of undefined type "
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