#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<Silent::SilentToken*> TokenPtrList;
typedef std::vector<SilentNode*> NodePtrList;
#define DEBUG 1

SilentParserInfo* info;
SilentToken ct; //current token
TokenList* tokensPtr;
uint64 cursor;

bool checkGlobalIdentifier(std::string name)
{
    for(auto function : info->namespaces[0]->functions)
    {
        if(function->name == name) return true;
    }

    for(auto structure : info->namespaces[0]->types)
    {
        if(structure->name == name) return true;
    }

    return false;
}

SilentNamespace* getNamespace(SilentNamespace& scope, std::string name)
{
    for(auto namespaceObj : scope.namespaces)
    {
        if(namespaceObj->name == name) return namespaceObj;
    }
    return NULL;
}

SilentStructure* getStruct(SilentNamespace& scope, std::string name)
{
    for(auto structure : scope.types)
    {
        if(structure->name == name) return structure;
    }
    return NULL;
}

SilentFunction* getFunction(SilentNamespace& scope, std::string name)
{
    for(auto function : scope.functions)
    {
        if(function->name == name) return function;
    }
    return NULL;
}


SilentDataType getType(SilentNamespace &scope, std::string name)
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
        if(getStruct(scope, name) != NULL) return SilentDataType::structType;
    }
    return SilentDataType::undefined;
}

uint64 getTypeSize(SilentNamespace &scope, std::string name)
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
        SilentStructure* structure = getStruct(scope,name);
        if(structure != NULL) return structure->size;
    }
    return -1;
}

// uint64 getLocalPos(NodePtrList &scope)
// {
//     uint64 scopeSize = scope.size();
//     uint64 lastPos = 1;
//     if(scopeSize == 0) return 0;
//     else
//     {
//         while(scope[scopeSize-lastPos]->type==SilentNodeType::structure)
//         {
//             lastPos+=1;
//         }
//         SilentVariable* var = scope[scopeSize-lastPos]->variable;
//         return var->localPos + var->size;
//     }
// }

void errorMsg(std::string msg, bool ex)
{
    std::cout << "Error on line: " << ct.line << "\n";
    std::cout << msg.data() << "\n";
    std::cout << "At token: " << ct.value.data() << "\n";
    if(ex){exit(-1);}
}

void nextToken()
{
    cursor += 1;
    if(cursor == tokensPtr->size())
    {
        cursor -= 1;
    }
    ct = (*tokensPtr)[cursor];
}

bool acceptToken(SilentTokenType type)
{
    if(ct.type == type)
    {
        return true;
    }
    return false;
}

bool expectToken(SilentTokenType type, std::string msg)
{
    if(acceptToken(type))
    {
        return true;
    }
    errorMsg(msg, false);
    return false;
}

SilentOperand* parseExpression();
SilentOperand* parseFactor()
{
    #if DEBUG
        std::cout << ct.value.data() << "\n";
    #endif
    if(acceptToken(SilentTokenType::Number))
    {
        SilentOperand* operand = new SilentOperand();
        operand->type = SilentOperandType::Value;
        operand->tokenData = new SilentToken;
        *(operand->tokenData) = ct;
        nextToken();
        return operand;
    }
    else if(acceptToken(SilentTokenType::Identifier))
    {
        SilentOperand* operand = new SilentOperand();
        operand->type = SilentOperandType::Identifier;
        nextToken();
        return operand;
    }
    else if(acceptToken(SilentTokenType::OpenParam))
    {
        nextToken();
        SilentOperand* operand = parseExpression();
        expectToken(SilentTokenType::CloseParam, "Expected closing parenthese");
        nextToken();
        return operand;
    }
    errorMsg("Syntax error", false);
    nextToken();
    return NULL;
}

SilentOperand* parseTerm()
{
    SilentOperand* operand = new SilentOperand();
    SilentOperand* temp;
    operand->left = parseFactor();

    while(true)
    {
        if(acceptToken(SilentTokenType::Multiply))
        {
            operand->type = SilentOperandType::Multiply;
            nextToken();
            operand->right = parseFactor();
            temp = operand;
            operand = new SilentOperand();
            operand->left = temp;
        }
        else if(acceptToken(SilentTokenType::Divide))
        {
            operand->type = SilentOperandType::Divide;
            nextToken();
            operand->right = parseFactor();
            temp = operand;
            operand = new SilentOperand();
            operand->left = temp;
        }
        else
        {
            break;
        }
    }
    temp = operand->left;
    delete operand;
    return temp;
}

SilentOperand* parseExpression()
{
    
    if(acceptToken(SilentTokenType::Add) ||
        acceptToken(SilentTokenType::Subtract)
    )
    {
        nextToken();
    }

    SilentOperand* operand = new SilentOperand();
    SilentOperand* temp;
    operand->left = parseTerm();
    while(true)
    {
        if(acceptToken(SilentTokenType::Add))
        {
            operand->type = SilentOperandType::Add;
            nextToken();
            operand->right = parseTerm();
            temp = operand;
            operand = new SilentOperand();
            operand->left = temp;
            
        }
        else if(acceptToken(SilentTokenType::Subtract))
        {
            operand->type = SilentOperandType::Subtract;
            nextToken();
            operand->right = parseTerm();
            temp = operand;
            operand = new SilentOperand();
            operand->left = temp;
        }
        else
        {
            break;
        }
    }
    temp = operand->left;
    delete operand;
    return temp;
}

SilentNode* Silent::SilentParseVar(
    NodePtrList &scope,
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
        errorMsg("Use of undefined type", false);
    }

    else if(node->variable->type == SilentDataType::structType)
    {
        node->variable->typePtr = 
        getNode(scope, ct.value,SilentNodeType::structure);  
    }
    node->variable->localPos = getLocalPos(scope);
    nextToken();

    if(getNode(scope, ct.value, SilentNodeType::variable) != NULL)
        errorMsg("Identifier " + ct.value + " already in use", true);
    else{node->name = ct.value;}
    nextToken();

    if(init)
    {
        if(expectEnd)
        {
            if (!acceptToken(SilentTokenType::Semicolon))
                errorMsg("Expected \";\" at the end of declaration", true);
            nextToken();
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
        
        if(acceptToken(SilentTokenType::Assign))
        {
            node->variable->expresion = parseExpression();
            if(expectToken(SilentTokenType::Semicolon,
                "Expected \";\" at the end of expression"))
            {
                nextToken();
                #if DEBUG
                    SilentPrintTree(node->variable->expresion);
                #endif
                return node;
            }
            nextToken();
        }
        else if(acceptToken(SilentTokenType::Semicolon))
        {
            node->variable->initialised = false;
            nextToken();
            return node;
        }
        else errorMsg("Invalid token following an expression", true);
    }
}

SilentNode* Silent::SilentParseStruct(NodePtrList &scope)
{
    SilentNode* node = new SilentNode();
    node->structure = new SilentStructure();
    node->type = SilentNodeType::structure;
    //NodePtrList* localScope = &(node->structure->variables);
    NodePtrList localScope;
    localScope = node->structure->variables;
    nextToken();

    if(getType(scope, ct.value) != SilentDataType::undefined)
        errorMsg("Identifier " + ct.value + " already in use", true);

    else node->name = ct.value;
    nextToken();
    expectToken(SilentTokenType::OpenScope, "Expected struct declaration");
    nextToken();

    while(!acceptToken(SilentTokenType::CloseScope))
    {
        if(getType(scope, ct.value) != SilentDataType::undefined)
        {
            SilentNode* var = SilentParseVar(
                localScope, ct.value, true, true
            );
            localScope.push_back(var);
            node->structure->size += var->variable->size;
            #if DEBUG
                std::cout << "struct size " << node->structure->size << "\n";
            #endif
        }
        else errorMsg("Use of non-existing type", true);
    }

    nextToken();

    #if DEBUG
        std::cout
        << "structure " << node->name.data() << " of size: " 
        << node->structure->size
        << " declared with variables:\n";

        for(uint64 i = 0; i < localScope.size(); i++)
        {
            std::cout << i << ":" << localScope[i]->name.data() << "\n";
        }
        std::cout << "\n";
    #endif
    return node;
}

void Silent::SilentParseParameters(SilentFunction* function, NodePtrList &scope)
{
    while(true)
    {
        function->parameters.push_back(
            SilentParseVar(function->parameters, ct.value,true,false)
        );
        if(ct.value == ",")
        {
            nextToken();
            continue;
        }
        else if(ct.type == SilentTokenType::CloseParam)
        {
            nextToken();
            break;
        }
        else errorMsg("Got invalid token whilst parsing parameters", false);
    }
}

SilentLocalScope* SilentParseLocalScope(SilentNamespace &scope)
{
    SilentNode* newScope = new SilentNode();
    newScope->type = SilentNodeType::scope;
    newScope->scope = new SilentScope();
    
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        if(acceptToken(SilentTokenType::Primitive))
        {
            SilentNode* node = SilentParseVar(
                newScope->scope->nodes, ct.value,false,true
            );
            newScope->scope->nodes.push_back(node);
        }
        else if(acceptToken(SilentTokenType::Identifier))
        {

        }
    }
    nextToken();
    return newScope;
}

SilentLocalScope* SilentParseFunctionScope(SilentNamespace& scope)
{
    SilentLocalScope* localScope = new SilentLocalScope();
    localScope->scopeParent = false;
    localScope->namespaceParent = &scope;
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        switch(ct.type)
        {
            case SilentTokenType::Identifier:
            case SilentTokenType::Primitive:
                localScope->variables.push_back(
                    SilentParseVar(*localScope, ct.value, false, true)
                );
            break;
        }
    }
}

SilentFunction* Silent::SilentParseFunction(SilentNamespace& scope)
{
    //Create function
    SilentFunction* function = new SilentFunction();

    //Get function return type
    nextToken();
    function->returnType = getType(scope, ct.value);
    if(function->returnType == SilentDataType::undefined)
        errorMsg("Use of undefined type", true);

    //Get function name
    nextToken();
    if(getFunction(scope, ct.value) != NULL)
        errorMsg("Identifier already in use", true);
    else function->name = ct.value;

    //Get function parameters
    nextToken();
    if(!acceptToken(SilentTokenType::OpenParam))
        errorMsg("Expected \"(\" for parameter declaration", true);
    nextToken();
    SilentParseParameters(*function);

    //Parse function scope
    if(!acceptToken(SilentTokenType::OpenScope))
    {
        function->initialised = false;
        if(!acceptToken(SilentTokenType::Semicolon))
        {
            errorMsg("Expected \";\" at the end of uninitialised function", false);
        }
        nextToken();
    }
    else
    {
        function->initialised = true;
        nextToken();
        function->scope = SilentParseFunctionScope();
    }

    #if DEBUG
        std::cout << "Declared function " << function->name.data() << "\n";
    #endif

    nextToken();
    return function;
}

SilentNamespace* Silent::SilentParseNamespace(SilentNamespace& scope)
{
    //Get namespace name
    nextToken();
    expectToken(SilentTokenType::Identifier, "Expected scope name");
    for(SilentNamespace* name : scope.namespaces)
    {
        if(name->name == scope.name) errorMsg("Conflicting namespaces", false);
    }
    nextToken();
    expectToken(SilentTokenType::OpenScope, "Expected scope declaration");
    nextToken();

    while(!acceptToken(SilentTokenType::CloseScope))
    {
        switch(ct.type)
        {
            case SilentTokenType::Namespace:
                scope.namespaces.push_back(
                    SilentParseNamespace(scope)
                );
                scope.namespaces.back()->parent = &scope;
            break;

            case SilentTokenType::Struct:
                scope.types.push_back(
                    SilentParseStruct(scope)
                );
            break;

            case SilentTokenType::Function:
                scope.functions.push_back(
                    SilentParseFunction(scope)
                );
            break; 
        }
    }
}

//Parse tokens
SilentParserInfo* Silent::SilentParse(TokenList tokens)
{   
    SilentParserInfo* pInfo = new SilentParserInfo();
    info = pInfo;
    ct = tokens[cursor];
    tokensPtr = &tokens;

    SilentNamespace* globalNamespace = new SilentNamespace();
    globalNamespace->name = "global";
    info->namespaces.push_back(globalNamespace);

    while(cursor < tokens.size()-1)
    {
        switch(ct.type)
        {
            case SilentTokenType::Namespace:
                globalNamespace->namespaces.push_back(
                    SilentParseNamespace(*globalNamespace)
                );
            break;

            case SilentTokenType::Struct:
                globalNamespace->types.push_back(
                    SilentParseStruct(*globalNamespace)
                );
            break;

            case SilentTokenType::Function:
                globalNamespace->functions.push_back(
                    SilentParseFunction(*globalNamespace)
                );
            break;

            // case SilentTokenType::Identifier:
            // case SilentTokenType::Primitive:
            //     globalNamespace->globals.push_back(
            //         SilentParseVar(globalNamespace, ct.value, false, true)
            //     );
            // break;

            default:
                errorMsg("Unexpected token in the global scope", true);
            break;
        }
    }
    return pInfo;
}