#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
#include "SilentDebugger.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<Silent::SilentToken*> TokenPtrList;
//typedef std::vector<Silent::SilentStructure*> TypePtrList;
//typedef std::vector<Silent::SilentFunction*> FuncPtrList;
#define DEBUG 1

struct ParserData
{
    SilentParserInfo* info;
    SilentToken ct; //current token
    TokenList* tokensPtr;
    uint64 cursor;
    std::vector<SilentNamespace*> accessibleNamespaces;
};
ParserData pd;

void errorMsg(std::string msg, bool ex)
{
    std::cout << "Error on line: " << pd.ct.line << "\n";
    std::cout << msg.data() << "\n";
    std::cout << "At token: " << pd.ct.value.data() << "\n";
    if(ex){exit(-1);}
}

SilentNamespace* getNamespace(std::string name)
{
    for(uint64 i = pd.accessibleNamespaces.size()-1; i >= 0; i--)
    {
        SilentNamespace* scope = pd.accessibleNamespaces[i];
        if(scope->name == name) return scope;
    }
    //errorMsg("Use of undefined namespace", false);
    return NULL;
}

SilentStructure* getStruct(std::string name)
{
    for(uint64 i = pd.accessibleNamespaces.size()-1; i >= 0; i--)
    {
        SilentNamespace* scope = pd.accessibleNamespaces[i];
        for(Silent::SilentStructure* structure : scope->types)
        {
            if(structure->name == name) return structure;
        }
    }
    //errorMsg("Use of undefined type", false);
    return NULL;
}

SilentDataType getType(std::string name)
{
    SilentDataType dataType;
    dataType.isPrimitive = true;
    if(name == "int8") dataType.primitive = SilentPrimitives::int8;
    else if(name == "uint8") dataType.primitive = SilentPrimitives::uint8;
    else if(name == "int16") dataType.primitive = SilentPrimitives::int16;
    else if(name == "uint16") dataType.primitive = SilentPrimitives::uint16;
    else if(name == "int32") dataType.primitive = SilentPrimitives::int32;
    else if(name == "uint32") dataType.primitive = SilentPrimitives::uint32;
    else if(name == "int64") dataType.primitive = SilentPrimitives::int64;
    else if(name == "uint64") dataType.primitive = SilentPrimitives::uint64;
    else if(name == "float32") dataType.primitive = SilentPrimitives::float32;
    else if(name == "float64") dataType.primitive = SilentPrimitives::float64;
    else if(name == "string") dataType.primitive = SilentPrimitives::string;
    else if(name == "pointer") dataType.primitive = SilentPrimitives::pointer;
    else if(name == "void") dataType.primitive = SilentPrimitives::null;
    else
    {
        dataType.type = getStruct(name);
        if(dataType.type == NULL) 
            dataType.primitive = SilentPrimitives::undefined;
        else dataType.isPrimitive = false;
        
    }
    return dataType;
}

SilentFunction* getLocalFunction(std::string name)
{
    SilentNamespace* scope = pd.accessibleNamespaces.back();
    for(Silent::SilentFunction* function : scope->functions)
    {
        if(function->name == name) return function;
    }
    //errorMsg("Function is not undefined", false);
    return NULL;
}

SilentFunction* getFunction(std::string name)
{
    for(uint64 i = pd.accessibleNamespaces.size()-1; i >= 0; i--)
    {
        SilentNamespace* scope = pd.accessibleNamespaces[i];
        for(Silent::SilentFunction* function : scope->functions)
        {
            if(function->name == name) return function;
        }
    }
    //errorMsg("Function is not undefined", false);
    return NULL;
}

uint64 getTypeSize(std::string name)
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
    else return getType(name).type->size;
    return -1;
}

bool isPrimitive(std::string name)
{
    if(name == "int8") return true;
    else if(name == "uint8") return true;
    else if(name == "int16") return true;
    else if(name == "uint16") return true;
    else if(name == "int32") return true;
    else if(name == "uint32") return true;
    else if(name == "int64") return true;
    else if(name == "uint64") return true;
    else if(name == "float32") return true;
    else if(name == "float64") return true;
    else if(name == "string") return true;
    else if(name == "pointer") return true;
    else if(name == "void") return true;
    else return false;
}

bool isValidType(std::string name)
{
    if(isPrimitive(name)) return true;
    else
    {
        for(SilentNamespace* scope : pd.accessibleNamespaces)
        {
            for(Silent::SilentStructure* structure : scope->types)
            {
                if(structure->name == name) return true;
            }
        }
        return false;
    }
}

uint64 getLocalPos(SilentLocalScope &scope)
{
    uint64 scopeSize = scope.variables.size();
    uint64 localPos = 0;
    for(auto var : scope.variables) localPos += var->size;
    return localPos;
}

void nextToken()
{
    pd.cursor += 1;
    if(pd.cursor == pd.tokensPtr->size())
    {
        pd.cursor -= 1;
    }
    pd.ct = (*pd.tokensPtr)[pd.cursor];
}

bool acceptToken(SilentTokenType type)
{
    if(pd.ct.type == type)
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
        std::cout << pd.ct.value.data() << "\n";
    #endif
    if(acceptToken(SilentTokenType::Number))
    {
        SilentOperand* operand = new SilentOperand();
        operand->type = SilentOperandType::Number;
        operand->token = new SilentToken;
        *(operand->token) = pd.ct;
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
    SilentLocalScope &scope, bool init, bool expectEnd
)
{
    #if DEBUG 
    std::cout << "Parsing var\n";
    #endif

    SilentVariable* var = new SilentVariable();

    var->type = getType(pd.ct.value);
    var->size = getTypeSize(pd.ct.value);
    var->localPos = getLocalPos(scope);

    nextToken();
    var->name = pd.ct.value;
    nextToken();

    //If only initialisation is permitted (structures etc)
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
        std::cout << "Finished parsing var " << var->name << "\n\n";
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
                var->initialised = true;
                nextToken();
                #if DEBUG
                    std::cout << "Syntax tree:\n";
                    SilentPrintTree(var->expresion);
                    std::cout << "Finished parsing var " << var->name << "\n\n";
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
            std::cout << "Finished parsing var " << var->name << "\n\n";
            #endif
            return var;
        }
        else errorMsg("Invalid token following an expression", true);
        #if DEBUG
        std::cout << "Finished parsing var " << var->name << "\n\n";
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
    if(isValidType(pd.ct.value))
        errorMsg("Identifier " + pd.ct.value + " already in use", true);
    else structure->name = pd.ct.value;
    nextToken();
    expectToken(SilentTokenType::OpenScope, "Expected struct declaration");
    nextToken();

    //Parse structure body
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        if(getType(pd.ct.value).primitive != SilentPrimitives::undefined)
        {
            SilentStatement* statement = new SilentStatement();
            statement->type = SilentStatementType::VarInit;

            SilentVariable* var = SilentParseVar(
                *structure->variables, true, true
            );
            structure->variables->variables.push_back(var);
            structure->variables->statements.push_back(statement);
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
    //accessibleTypes.back().push_back(structure);
    return structure;
}

SilentLocalScope* Silent::SilentParseParameters(SilentNamespace &scope)
{
    #if DEBUG
        std::cout << "Parsing parameters\n";
    #endif
    SilentLocalScope* parameters = new SilentLocalScope();
    while(pd.ct.type != SilentTokenType::CloseParam)
    {
        SilentStatement* statement = new SilentStatement();
        statement->type = SilentStatementType::VarInit;
        parameters->variables.push_back(SilentParseVar(*parameters,true,false));
        parameters->statements.push_back(statement);
        if(pd.ct.value == ",") {nextToken(); continue;}
        else if(pd.ct.type == SilentTokenType::CloseParam) break;
        else errorMsg("Got invalid token whilst parsing parameters", false);
    }
    nextToken();
    #if DEBUG
        std::cout << "finished parsing parameters\n\n";
    #endif
    return parameters;
}

SilentLocalScope* Silent::SilentParseLocalScope(SilentNamespace &scope)
{
    #if DEBUG
    std::cout << "Parsing local scope\n";
    #endif
    SilentLocalScope* localScope = new SilentLocalScope();
    localScope->usesScopeParent = false;
    localScope->namespaceParent = &scope;
    
    while(!acceptToken(SilentTokenType::CloseScope))
    {
        switch(pd.ct.type)
        {
            case SilentTokenType::Identifier:
            case SilentTokenType::Primitive:
                SilentStatement* statement = new SilentStatement();
                statement->type = SilentStatementType::VarInit;

                SilentVariable* var = 
                    SilentParseVar(*localScope, false, true);

                localScope->variables.push_back(var);
                if(var->initialised) 
                    localScope->statements.push_back(statement);
            break;

            //Add other statements
        }
    }
    //nextToken();
    #if DEBUG
    std::cout << "Finished parsing local scope\n\n";
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
    function->returnType = getType(pd.ct.value);

    //Get function name
    nextToken();
    if(getLocalFunction(pd.ct.value) != NULL)
        errorMsg("Identifier already in use locally", true);
    else function->name = pd.ct.value;
    if(pd.ct.value == "main")
    {
        if(pd.info->main == NULL) pd.info->main = function;
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
        function->scope = new SilentLocalScope();
        if(!acceptToken(SilentTokenType::Semicolon))
        {
            errorMsg("Expected \";\" at the end of uninitialised function", false);
        }
        //nextToken();
    }
    else
    {
        function->initialised = true;
        nextToken();
        function->scope = SilentParseLocalScope(scope);
        std::cout << "v: "<<function->scope->variables.size()<<"\n";
        std::cout << "s: "<<function->scope->statements.size()<<"\n";
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
    pd.accessibleNamespaces.push_back(newNamespace);

    //Get namespace name
    nextToken();
    expectToken(SilentTokenType::Identifier, "Expected scope name");
    newNamespace->name = pd.ct.value;
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
        switch(pd.ct.type)
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
    nextToken();
    pd.accessibleNamespaces.pop_back();

    #if DEBUG
    std::cout << "Finished parsing namespace: " << newNamespace->name << "\n\n";
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
    pd.info = pInfo;
    pd.ct = tokens[pd.cursor];
    pd.tokensPtr = &tokens;

    SilentNamespace* globalNamespace = new SilentNamespace();
    globalNamespace->globals = new SilentLocalScope();
    globalNamespace->globals->namespaceParent = globalNamespace;
    globalNamespace->globals->usesScopeParent = false;
    globalNamespace->name = "global";
    pd.accessibleNamespaces.push_back(globalNamespace);
    pd.info->globalNamespace = globalNamespace;

    while(pd.cursor < tokens.size()-1)
    {
        switch(pd.ct.type)
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
                    SilentParseVar(*globalNamespace->globals, false, true)
                );
            break;

            default:
                errorMsg("Unexpected token in the global scope", true);
            break;
        }
    }
    pd.accessibleNamespaces.pop_back();
    #if DEBUG
    std::cout << "Finished parsing\n\n";
    #endif
    return pInfo;
}