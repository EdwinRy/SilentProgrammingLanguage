#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
#include "SilentDebugger.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<Silent::SilentToken*> TokenPtrList;
#define DEBUG 0

SilentParserInfo* info;
SilentToken ct; //current token
TokenList* tokensPtr;
uint64 cursor;

bool checkGlobalIdentifier(std::string name)
{
    for(auto function : info->namespaces[0]->functions)
        if(function->name == name) return true;

    for(auto structure : info->namespaces[0]->types)
        if(structure->name == name) return true;

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

uint64 getLocalPos(SilentLocalScope &scope)
{
    uint64 scopeSize = scope.variables.size();
    uint64 localPos = 0;
    for(auto var : scope.variables) localPos += var->size;
    return localPos;
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
        operand->type = SilentOperandType::Number;
        operand->token = new SilentToken;
        *(operand->token) = ct;
        nextToken();
        return operand;
    }
    else if(acceptToken(SilentTokenType::Identifier))
    {
        SilentOperand* operand = new SilentOperand();
        operand->type = SilentOperandType::Variable;
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
    //delete operand;
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
    //delete operand;
    return temp;
}

SilentVariable* Silent::SilentParseVar(
    SilentLocalScope &scope,
    SilentNamespace &typeScope,
    std::string type,
    bool init,
    bool expectEnd
)
{
    #if DEBUG 
    std::cout << "Parsing var\n";
    #endif

    SilentVariable* var = new SilentVariable();

    //Get variable type
    var->type = getType(typeScope, ct.value);


    if(var->type == SilentDataType::undefined)
        errorMsg("Use of undefined type", false);

    //Get variable size
    var->size = getTypeSize(typeScope, ct.value);

    //Get variable position locally
    var->localPos = getLocalPos(scope);
    nextToken();

    //Get variable name
    var->name = ct.value;
    nextToken();

    //If only initialisation permitted
    if(init)
    {
        //If semicolon required at the end
        if(expectEnd)
        {
            if (!acceptToken(SilentTokenType::Semicolon))
                errorMsg("Expected \";\" at the end of declaration", true);
            nextToken();
        }
        #if DEBUG
            std::cout << "Declared variable " << var->name.data() << "\n"
            << "of size: " << var->size << "\n"
            << "at local position " << var->localPos << "\n";
        std::cout << "Finished parsing var\n\n";
        #endif
        return var;
    }
    else
    {
        //Parse statement
        if(acceptToken(SilentTokenType::Assign))
        {    
            nextToken();
            var->expresion = new SilentOperand();
            var->expresion->type = SilentOperandType::Assign;
            var->expresion->left = new SilentOperand();
            var->expresion->left->type = SilentOperandType::Variable;
            var->expresion->left->variable = var;
            var->expresion->right = parseExpression();
            if(expectToken(SilentTokenType::Semicolon,
                "Expected \";\" at the end of expression")
            )
            {
                nextToken();
                #if DEBUG
                    std::cout << "Syntax tree:\n";
                    SilentPrintTree(var->expresion);
                    std::cout << "Finished parsing var\n\n";
                #endif
                return var;
            }
            nextToken();
        }
        //End declaration
        else if(acceptToken(SilentTokenType::Semicolon))
        {
            var->initialised = false;
            nextToken();
            #if DEBUG
            std::cout << "Finished parsing var\n\n";
            #endif
            return var;
        }
        else errorMsg("Invalid token following an expression", true);
        #if DEBUG
        std::cout << "Finished parsing var\n\n";
        #endif
        return var;
    }
}

SilentStructure* Silent::SilentParseStruct(SilentNamespace &scope)
{
    #if DEBUG
    std::cout << "Parsing struct\n";
    #endif
    SilentStructure* structure = new SilentStructure();
    structure->variables = new SilentLocalScope();

    //Get structure name
    nextToken();
    if(getType(scope, ct.value) != SilentDataType::undefined)
        errorMsg("Identifier " + ct.value + " already in use", true);
    else structure->name = ct.value;
    nextToken();
    expectToken(SilentTokenType::OpenScope, "Expected struct declaration");
    nextToken();

    //Parse structure body
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        if(getType(scope, ct.value) != SilentDataType::undefined)
        {
            SilentVariable* var = SilentParseVar(
                *structure->variables, scope, ct.value, true, true
            );
            structure->variables->variables.push_back(var);
            structure->size += var->size;
            #if DEBUG
                std::cout << "struct size " << structure->size << "\n";
            #endif
        }
        else errorMsg("Use of non-existing type", true);
    }

    nextToken();
    #if DEBUG
        std::cout
        << "structure " << structure->name.data() << " of size: " 
        << structure->size
        << " declared with variables:\n";
        for(uint64 i = 0; i < structure->variables->variables.size(); i++)
        {
            std::cout << i << ":" << 
            structure->variables->variables[i]->name.data() << "\n";
        }
        std::cout << "\n";
        std::cout << "Finished parsing struct\n\n";
    #endif
    return structure;
}

SilentLocalScope* Silent::SilentParseParameters(SilentNamespace &scope)
{
    #if DEBUG
    std::cout << "Parsing parameters\n";
    #endif
    SilentLocalScope* parameters = new SilentLocalScope();
    while(ct.type != SilentTokenType::CloseParam)
    {
        parameters->variables.push_back(
            SilentParseVar(*parameters, scope, ct.value,true,false)
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
    #if DEBUG
    std::cout << "finished parsing parameters\n\n";
    #endif
    return parameters;
}

SilentLocalScope* SilentParseLocalScope(SilentNamespace &scope)
{
    #if DEBUG
    std::cout << "Parsing local scope\n";
    #endif
    SilentLocalScope* localScope = new SilentLocalScope();
    localScope->usesScopeParent = false;
    localScope->namespaceParent = &scope;
    
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        switch(ct.type)
        {
            case SilentTokenType::Primitive:
                localScope->variables.push_back(
                    SilentParseVar(*localScope, scope, ct.value, false, true)
                );
            break;

            //Add other statements

        }
    }
    nextToken();
    #if DEBUG
    std::cout << "Finished parsing local scope\n\n";
    #endif
    return localScope;
}

SilentLocalScope* Silent::SilentParseFunctionScope(SilentNamespace& scope)
{
    #if DEBUG
    std::cout << "Parsing function scope\n";
    #endif
    SilentLocalScope* localScope = new SilentLocalScope();
    localScope->usesScopeParent = false;
    localScope->namespaceParent = &scope;
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        switch(ct.type)
        {
            case SilentTokenType::Identifier:
            case SilentTokenType::Primitive:
                localScope->variables.push_back(
                    SilentParseVar(*localScope, scope, ct.value, false, true)
                );
            break;

            //Add other statements

        }
    }
    #if DEBUG
    std::cout << "Finished parsing function scope\n\n";
    #endif
    return localScope;
}

SilentFunction* Silent::SilentParseFunction(SilentNamespace& scope)
{
    #if DEBUG
    std::cout << "Parsing function\n";
    #endif
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
    if(ct.value == "main")
    {
        if(info->main == NULL) info->main = function;
        else errorMsg("Main function already declared", false);
    }

    //Get function parameters
    nextToken();
    if(!acceptToken(SilentTokenType::OpenParam))
        errorMsg("Expected \"(\" for parameter declaration", true);
    nextToken();
    function->parameters = SilentParseParameters(scope);

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
        function->scope = SilentParseFunctionScope(scope);
    }

    #if DEBUG
        std::cout << "Declared function " << function->name.data() << "\n";
    #endif

    nextToken();
    #if DEBUG
    std::cout << "Finished parsing function\n\n";
    #endif
    return function;
}

SilentNamespace* Silent::SilentParseNamespace(SilentNamespace& scope)
{
    #if DEBUG
    std::cout << "Parsing namespace\n";
    #endif
    SilentNamespace* newNamespace = new SilentNamespace();
    //Get namespace name
    nextToken();
    expectToken(SilentTokenType::Identifier, "Expected scope name");
    newNamespace->name = ct.value;
    for(SilentNamespace* name : scope.namespaces)
    {
        if(name->name == newNamespace->name) 
            errorMsg("Conflicting namespaces", false);
    }
    nextToken();
    expectToken(SilentTokenType::OpenScope, "Expected scope declaration");
    nextToken();

    while(!acceptToken(SilentTokenType::CloseScope))
    {
        switch(ct.type)
        {
            case SilentTokenType::Namespace:
                newNamespace->namespaces.push_back(
                    SilentParseNamespace(*newNamespace)
                );
                newNamespace->namespaces.back()->parent = newNamespace;
            break;

            case SilentTokenType::Struct:
                newNamespace->types.push_back(
                    SilentParseStruct(*newNamespace)
                );
            break;

            case SilentTokenType::Function:
                newNamespace->functions.push_back(
                    SilentParseFunction(*newNamespace)
                );
            break; 
        }
    }
    #if DEBUG
    std::cout << "Finished parsing namespace\n\n";
    #endif
    return newNamespace;
}

//Parse tokens
SilentParserInfo* Silent::SilentParse(TokenList tokens)
{   
    #if DEBUG
    std::cout << "Started parsing\n";
    #endif
    SilentParserInfo* pInfo = new SilentParserInfo();
    info = pInfo;
    ct = tokens[cursor];
    tokensPtr = &tokens;

    SilentNamespace* globalNamespace = new SilentNamespace();
    globalNamespace->globals = new SilentLocalScope();
    globalNamespace->globals->namespaceParent = globalNamespace;
    globalNamespace->globals->usesScopeParent = false;
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

            case SilentTokenType::Identifier:
            case SilentTokenType::Primitive:
                globalNamespace->globals->variables.push_back(
                    SilentParseVar(*globalNamespace->globals, *globalNamespace,
                        ct.value, false, true
                    )
                );
            break;

            default:
                errorMsg("Unexpected token in the global scope", true);
            break;
        }
    }
    #if DEBUG
    std::cout << "Finished parsing\n\n";
    #endif
    return pInfo;
}