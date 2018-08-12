#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<Silent::SilentToken*> TokenPtrList;
//typedef std::vector<SilentNode> NodeList;
typedef std::vector<SilentNode*> NodePtrList;
#define DEBUG 1

SilentParserInfo* info;
SilentToken ct; //current token
TokenList* tokensPtr;
uint64 cursor;

bool checkGlobalIdentifier(std::string name)
{
    for(auto function : info->functions)
    {
        if(function->name == name) return true;
    }

    for(auto structure : info->types)
    {
        if(structure->name == name) return true;
    }

    return false;
}


SilentNode* getNode(NodePtrList &scope, std::string name, SilentNodeType type)
{
    //for(uint64 i = 0; i < (*scope).size(); i++)
    for(uint64 i = 0; i < scope.size(); i++)
    {
        if(scope[i]->type == type)
        {
            if(scope[i]->name == name) return scope[i];
        }
    }
    return NULL;
}

SilentNode* getAccessibleNode(std::string name, SilentNodeType type)
{
    //Scan from the end to get the closest variable
    for(uint64 i = info->nodes.size()-1; i >= 0; i--)
    {
        if(info->nodes[i]->type == type)
        {
            if(info->nodes[i]->name == name) return info->nodes[i];
        }
    }
    return NULL;
}

SilentDataType getType(NodePtrList &scope, std::string name)
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
        if(getNode(scope, name, SilentNodeType::structure) != NULL) 
        {
            return SilentDataType::structType;
        }
    }
    return SilentDataType::undefined;
}

uint64 getTypeSize(NodePtrList &scope, std::string name)
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

uint64 getLocalPos(NodePtrList &scope)
{
    uint64 scopeSize = scope.size();
    uint64 lastPos = 1;
    if(scopeSize == 0) return 0;
    else
    {
        while(scope[scopeSize-lastPos]->type==SilentNodeType::structure)
        {
            lastPos+=1;
        }
        SilentVariable* var = scope[scopeSize-lastPos]->variable;
        return var->localPos + var->size;
    }
}

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

SilentNode* Silent::SilentParseScope(NodePtrList &scope)
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

SilentNode* Silent::SilentParseFunction(NodePtrList& scope)
{
    SilentNode* node = new SilentNode();
    node->function = new SilentFunction();
    node->type = SilentNodeType::function;
    nextToken();

    node->function->returnType = getType(scope, ct.value);
    if(node->function->returnType == SilentDataType::undefined)
        errorMsg("Use of undefined type", true);

    nextToken();
    if(getNode(scope, ct.value, SilentNodeType::function) != NULL)
        errorMsg("Identifier already in use", true);

    else node->name = ct.value;
    nextToken();

    if(!acceptToken(SilentTokenType::OpenParam))
        errorMsg("Expected \"(\" for parameter declaration", true);


    nextToken();
    SilentParseParameters(node->function,scope);

    if(!acceptToken(SilentTokenType::OpenScope))
    {
        node->function->initialised = false;
        if(!acceptToken(SilentTokenType::Semicolon))
        {
            errorMsg("Expected \";\" at the end of uninitialised function", false);
        }
        nextToken();
    }
    else
    {
        node->function->initialised = true;
        nextToken();
        //node->function->scope = SilentParseScope(scope);
    }

    #if DEBUG
        std::cout << "Declared function " << node->name.data() << "\n";
    #endif
    nextToken();
    return node;
}

void GetGlobalIdentifiers(TokenList tokens)
{
    for(uint64 i = 0; i < tokens.size(); i++)
    {
        switch(tokens[i].type)
        {
            case SilentTokenType::Function:
            {
                SilentFunction* newF = new SilentFunction();
                newF->initialised = false;
                if(tokens[i+=2].type != SilentTokenType::Identifier)
                {
                    ct = tokens[i];
                    errorMsg("Function name needs to be a valid identifier",false);
                }
                if(checkGlobalIdentifier(tokens[i].value))
                {
                    ct = tokens[i];
                    errorMsg("Identifier already in use", false);
                }
                newF->name = tokens[i].value;
                info->functions.push_back(newF);
            }
            break;

            case SilentTokenType::Struct:
            {
                SilentStructure* newS = new SilentStructure();
                newS->initialised = false;
                if(tokens[++i].type != SilentTokenType::Identifier)
                {
                    ct = tokens[i];
                    errorMsg("Structure name needs to be a valid identifier",false);
                }
                if(checkGlobalIdentifier(tokens[i].value))
                {
                    ct = tokens[i];
                    errorMsg("Identifier name already in use", false);
                }
                newS->name = tokens[i].value;
                info->types.push_back(newS);
            }
            break;
        }
    }
}

//Parse tokens
SilentParserInfo* Silent::SilentParse(TokenList tokens)
{   
    SilentParserInfo* pInfo = new SilentParserInfo();
    info = pInfo;
    //NodePtrList* globalScope = new NodePtrList();
    SilentScope* globalScope = new SilentScope();
    ct = tokens[cursor];
    tokensPtr = &tokens;

    GetGlobalIdentifiers(tokens);

    while(cursor < tokens.size()-1)
    {
        //Parse structure declaration
        if(acceptToken(SilentTokenType::Struct))
        {
            SilentNode* node = SilentParseStruct(globalScope->nodes);
            globalScope->nodes.push_back(node);
        }

        //Parse function declaration
        else if(acceptToken(SilentTokenType::Function))
        {
            SilentNode* node = SilentParseFunction(globalScope->nodes);
            globalScope->nodes.push_back(node);
        }

        //Parse primitive variable
        else if(acceptToken(SilentTokenType::Primitive))
        {
            SilentNode* node = SilentParseVar(
                globalScope->nodes,ct.value,true,true
            );
            globalScope->nodes.push_back(node);
        }

        //Parse custom type variable
        else if(acceptToken(SilentTokenType::Identifier))
        {
            SilentDataType type = getType(globalScope->nodes, ct.value);
            if(type == SilentDataType::structType)
            {
                SilentNode* node = SilentParseVar(
                    globalScope->nodes,ct.value,true,true
                );
                globalScope->nodes.push_back(node);
            }
            else errorMsg("Use of undefined type", true);
        }

        //Display invalid token error
        else errorMsg("Unexpected token in the global scope", true);
    }
    return pInfo;
}