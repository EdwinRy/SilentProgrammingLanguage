#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
#include "SilentDebugger.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
#define DEBUG 1

namespace Silent
{
    SilentParser::SilentParser()
    {
        this->globalNamespace = new SilentNamespace();
        this->accessibleNamespaces = {};
    }
    SilentParser::~SilentParser(){delete this->globalNamespace;}
    bool SilentParser::SilentParse(
        std::vector<Silent::SilentToken> tokens
    )
    {
        #if DEBUG
        std::cout << "Started parsing\n";
        #endif

        tokenCursor = 0;
        ct = tokens[tokenCursor];
        tokensPtr = &tokens;

        globalNamespace->globals = new SilentLocalScope();
        //globalNamespace->globals->namespaceParent = globalNamespace;
        //globalNamespace->globals->usesScopeParent = false;
        globalNamespace->name = "global";
        accessibleNamespaces.push_back(globalNamespace);

        while(tokenCursor < tokens.size()-1)
        {
            switch(ct.type)
            {
                case SilentTokenType::Namespace:
                    globalNamespace->namespaces.push_back(
                        ParseNamespace(*globalNamespace)
                    );
                break;

                case SilentTokenType::Struct:
                    globalNamespace->types.push_back(
                        ParseStruct(*globalNamespace)
                    );
                break;

                case SilentTokenType::Function:
                    // globalNamespace->functions.push_back(
                    //     ParseFunction(*globalNamespace)
                    // );
                    ParseFunction(*globalNamespace);
                break;

                case SilentTokenType::Identifier:
                case SilentTokenType::Primitive:
                    if(IsValidType(ct.value))
                    {
                        SilentVariable* var = ParseVariable(
                            *globalNamespace->globals, true, true);
                        globalNamespace->globals->variables.push_back(var);
                        globalVarPointer += var->size;
                    }
                    else
                    {
                        ErrorMsg("Unexpected token in the global scope");
                        NextToken();
                    }
                break;

                default:
                    ErrorMsg("Unexpected token in the global scope");
                    NextToken();
                    return false;
                break;
            }
        }
        accessibleNamespaces.pop_back();
        #if DEBUG
        std::cout << "Finished parsing\n\n";
        #endif
        return true;
    }

    SilentNamespace* SilentParser::GetGlobalNamespace(){return globalNamespace;}

    void SilentParser::ErrorMsg(std::string msg)
    {
        std::cout << "Error on line: " << ct.line << "\n";
        std::cout << msg.data() << "\n";
        std::cout << "At token: " << ct.value.data() << "\n";
    }

    SilentNamespace* SilentParser::GetNamespace(std::string name)
    {
        for(uint64 i = accessibleNamespaces.size()-1; i >= 0; i--)
        {
            SilentNamespace* scope = accessibleNamespaces[i];
            if(scope->name == name) return scope;
        }
        return NULL;
    }

    SilentStructure* SilentParser::GetStructure(std::string name)
    {
        for(uint64 i = accessibleNamespaces.size()-1; i >= 0; i--)
        {
            SilentNamespace* scope = accessibleNamespaces[i];
            for(SilentStructure* structure : scope->types)
            {
                if(structure->name == name) return structure;
            }
        }
        return NULL;
    }

    SilentDataType SilentParser::GetType(std::string name)
    {
        SilentDataType dataType;
        dataType.isPrimitive = true;
        if(name == "int8") 
            dataType.primitive = SilentPrimitives::int8;
        else if(name == "uint8") 
            dataType.primitive = SilentPrimitives::uint8;
        else if(name == "int16") 
            dataType.primitive = SilentPrimitives::int16;
        else if(name == "uint16") 
            dataType.primitive = SilentPrimitives::uint16;
        else if(name == "int32") 
            dataType.primitive = SilentPrimitives::int32;
        else if(name == "uint32") 
            dataType.primitive = SilentPrimitives::uint32;
        else if(name == "int64") 
            dataType.primitive = SilentPrimitives::int64;
        else if(name == "uint64") 
            dataType.primitive = SilentPrimitives::uint64;
        else if(name == "float32") 
            dataType.primitive = SilentPrimitives::float32;
        else if(name == "float64") 
            dataType.primitive = SilentPrimitives::float64;
        else if(name == "string") 
            dataType.primitive = SilentPrimitives::string;
        else if(name == "pointer") 
            dataType.primitive = SilentPrimitives::pointer;
        else if(name == "void") 
            dataType.primitive = SilentPrimitives::null;
        else
        {
            dataType.type = GetStructure(name);
            if(dataType.type == NULL) 
                dataType.primitive = SilentPrimitives::undefined;
            else dataType.isPrimitive = false;
            
        }
        return dataType;
    }

    SilentFunction* SilentParser::GetLocalFunction(std::string name)
    {
        SilentNamespace* scope = accessibleNamespaces.back();
        for(SilentFunction* function : scope->functions)
        {
            if(function->name == name) return function;
        }
        return NULL;
    }

    SilentFunction* SilentParser::GetFunction(std::string name)
    {
        for(uint64 i = accessibleNamespaces.size()-1; i >= 0; i--)
        {
            SilentNamespace* scope = accessibleNamespaces[i];
            for(SilentFunction* function : scope->functions)
            {
                if(function->name == name) return function;
            }
        }
        return NULL;
    }

    uint64 SilentParser::GetTypeSize(std::string name)
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
        else return GetType(name).type->size;
        return -1;
    }

    uint64 SilentParser::GetLocalPos(SilentLocalScope &scope)
    {
        if(scope.variables.size() > 0)
        {
            SilentVariable* lastVar = scope.variables.back();
            return lastVar->localPos + lastVar->size;
        }
        else return 0;
    }

    SilentVariable* SilentParser::GetLocalVariable(
        SilentLocalScope &scope, std::string name)
    {
        for(SilentVariable* var : scope.variables)
        {
            if(var->name == name) return var;
        }
        return NULL;
    }

    SilentVariable* SilentParser::GetVariable(std::string name)
    {
        for(uint64 i = accessibleNamespaces.size()-1; i >= 0; i--)
        {
            SilentNamespace* scope = accessibleNamespaces[i];
            for(SilentFunction* function : scope->functions)
            {
                SilentLocalScope* localScope = function->scope;
                for(SilentVariable* var : localScope->variables)
                {
                    if(var->name == name) return var;
                }
            }
        }
        return NULL;
    }

    bool SilentParser::IsPrimitive(std::string name)
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

    bool SilentParser::IsValidType(std::string name)
    {
        if(IsPrimitive(name)) return true;
        else
        {
            for(SilentNamespace* scope : accessibleNamespaces)
            {
                for(Silent::SilentStructure* structure : scope->types)
                {
                    if(structure->name == name) return true;
                }
            }
            return false;
        }
    }

    

    void SilentParser::NextToken()
    {
        tokenCursor++;
        if(tokenCursor == tokensPtr->size()) tokenCursor--;
        ct = (*tokensPtr)[tokenCursor];
    }

    void SilentParser::PreviousToken()
    {
        tokenCursor--;
        if((long long)tokenCursor == -1) tokenCursor++;
        ct = (*tokensPtr)[tokenCursor];
    }

    SilentToken SilentParser::PeakToken()
    {
        return (*tokensPtr)[tokenCursor+1];
    }

    bool SilentParser::AcceptToken(SilentTokenType type)
    {
        return ct.type == type;
    }

    bool SilentParser::ExpectToken(SilentTokenType type, std::string msg)
    {
        if(AcceptToken(type)) return true;
        ErrorMsg(msg);
        return false;
    }


    SilentOperand* SilentParser::ParseFactor()
    {
        #if DEBUG
        std::cout << ct.value.data() << "\n";
        #endif

        switch(ct.type)
        {
            case SilentTokenType::Number:
            {
                SilentOperand* operand = new SilentOperand();
                operand->expressionType = SilentExpressionType::Data;
                operand->type = SilentOperandType::Number;
                operand->token = new SilentToken;
                *(operand->token) = ct;
                NextToken();
                return operand;
            }
            break;

            case SilentTokenType::Identifier:
            {
                SilentOperand* operand = new SilentOperand();
                operand->type = SilentOperandType::Variable;
                operand->expressionType = SilentExpressionType::Data;
                operand->variable = GetVariable(ct.value);
                if(operand->variable == NULL) 
                    ErrorMsg("Use of undeclared variable");
                //printf("GOTTEN VARIABLE %S\n",operand->variable->name.data());
                //operand->variable = GetLocalVariable()
                NextToken();
                return operand;
            }
            break;

            case SilentTokenType::OpenParam:
            {
                NextToken();
                SilentOperand* operand = ParseExpression();
                ExpectToken(SilentTokenType::CloseParam, "Expected token ')'");
                NextToken();
                return operand;
            }
            break;

            default:
                ErrorMsg("Syntax Error");
                NextToken();
                return NULL;
            break;
        }
    }

    SilentOperand* SilentParser::ParseTerm()
    {
        SilentOperand* operand = new SilentOperand();
        SilentOperand* temp;
        operand->left = ParseFactor();

        while(true)
        {
            if(AcceptToken(SilentTokenType::Multiply))
            {
                operand->type = SilentOperandType::Multiply;
                operand->expressionType = SilentExpressionType::Arithmetic;
                NextToken();
                operand->right = ParseFactor();
                temp = operand;
                operand = new SilentOperand();
                operand->left = temp;
            }
            else if(AcceptToken(SilentTokenType::Divide))
            {
                operand->type = SilentOperandType::Divide;
                operand->expressionType = SilentExpressionType::Arithmetic;
                NextToken();
                operand->right = ParseFactor();
                temp = operand;
                operand = new SilentOperand();
                operand->left = temp;
            }
            else break;
        }
        temp = operand->left;
        return temp;
    }

    SilentOperant* SilentParser::ParseSum()
    {
        switch(ct.type)
        {
            case SilentTokenType::Add:
            case SilentTokenType::Subtract:
                NextToken();
            break;
            default: break;
        }

        SilentOperand* operand = new SilentOperand();
        SilentOperand* temp;
        operand->left = ParseTerm();

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(ct.type)
            {
                case SilentTokenType::Add:
                {
                    operand->type = SilentOperandType::Add;
                    operand->expressionType = SilentExpressionType::Arithmetic;
                    NextToken();
                    operand->right = ParseTerm();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;

                case SilentTokenType::Subtract:
                {
                    operand->type = SilentOperandType::Subtract;
                    operand->expressionType = SilentExpressionType::Arithmetic;
                    NextToken();
                    operand->right = ParseTerm();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;

                default: parsingExpression = false; break;

            }
        }
        temp = operand->left;
        return temp;
    }

    SilentOperand* SilentParser::ParseLogic()
    {
        switch(ct.type)
        {
            case SilentTokenType::And:
            case SilentTokenType::Or:
            case SilentTokenType::Not: NextToken(); break;
            default: break;
        }

        SilentOperand* operand = new SilentOperand();
        SilentOperand* temp;
        operand->left = ParseSum();

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(ct.type)
            {
                case SilentTokenType::And:
                {
                    operand->type = SilentOperandType::And;
                    operand->expressionType = SilentExpressionType::Logical;
                    NextToken();
                    operand->right = ParseSum();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::Or:
                {
                    operand->type = SilentOperandType::Or;
                    operand->expressionType = SilentExpressionType::Logical;
                    NextToken();
                    operand->right = ParseSum();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::Xor:
                {
                    operand->type = SilentOperandType::Xor;
                    operand->expressionType = SilentExpressionType::Logical;
                    NextToken();
                    operand->right = ParseSum();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::Not:
                {
                    operand->type = SilentOperandType::Not;
                    operand->expressionType = SilentExpressionType::Logical;
                    NextToken();
                    operand->right = ParseSum();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                default: parsingExpression = false; break;

            }
        }
        temp = operand->left;
        return temp;
    }

    SilentOperand* SilentParser::ParseComparison()
    {
        switch(ct.type)
        {
            case SilentTokenType::Equal:
            case SilentTokenType::NotEqual:
            case SilentTokenType::Larger:
            case SilentTokenType::LargerOrEqual:
            case SilentTokenType::Smaller:
            case SilentTokenType::SmallerOrEqual: NextToken(); break;
            default: break;
        }

        SilentOperand* operand = new SilentOperand();
        SilentOperand* temp;
        operand->left = ParseLogic();

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(ct.type)
            {
                case SilentTokenType::Equal:
                {
                    operand->type = SilentOperandType::Equal;
                    operand->expressionType = SilentExpressionType::Comparison;
                    NextToken();
                    operand->right = ParseLogic();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::NotEqual:
                {
                    operand->type = SilentOperandType::NotEqual;
                    operand->expressionType = SilentExpressionType::Comparison;
                    NextToken();
                    operand->right = ParseLogic();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::Larger:
                {
                    operand->type = SilentOperandType::Larger;
                    operand->expressionType = SilentExpressionType::Comparison;
                    NextToken();
                    operand->right = ParseLogic();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::LargerOrEqual:
                {
                    operand->type = SilentOperandType::LargerOrEqual;
                    operand->expressionType = SilentExpressionType::Comparison;
                    NextToken();
                    operand->right = ParseLogic();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::Smaller:
                {
                    operand->type = SilentOperandType::Smaller;
                    operand->expressionType = SilentExpressionType::Comparison;
                    NextToken();
                    operand->right = ParseLogic();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;
                case SilentTokenType::SmallerOrEqual: 
                {
                    operand->type = SilentOperandType::SmallerOrEqual;
                    operand->expressionType = SilentExpressionType::Comparison;
                    NextToken();
                    operand->right = ParseLogic();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                }
                break;

                default: parsingExpression = false; break;

            }
        }
        temp = operand->left;
        return temp;
    }

    SilentOperand* SilentParser::ParseExpression()
    {
        if(AcceptToken(SilentTokenType::Assign)) NextToken();

        SilentOperand* operand = new SilentOperand();
        SilentOperand* temp;
        operand->left = ParseComparison();

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(ct.type)
            {
                case SilentTokenType::Assign:
                    operand->type = SilentOperandType::Assign;
                    operand->expressionType = SilentExpressionType::Memory;
                    NextToken();
                    operand->right = ParseComparison();
                    temp = operand;
                    operand = new SilentOperand();
                    operand->left = temp;
                break;

                default: parsingExpression = false; break;
            }
        }
        temp = operand->left;
        return temp;
    }

    SilentStatement* SilentParser::ParseStatement(SilentLocalScope &scope)
    {
        SilentStatement* statement = new SilentStatement();

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(ct.type)
            {
                case SilentTokenType::Identifier:
                    statement->type = SilentStatementType::Expression;
                    statement->expression = ParseExpression();
                break;

                case SilentTokenType::Semicolon:
                    parsingExpression = false;
                break;

                default:
                    parsingExpression = false;
                    ErrorMsg("Invalid statement"); 
                break;
            }
        }
        NextToken();
        return statement;
    }

    SilentVariable* SilentParser::ParseVariable(
        SilentLocalScope &scope, bool init, bool expectEnd
    )
    {
        #if DEBUG 
        std::cout << "Parsing variable\n";
        #endif

        SilentVariable* var = new SilentVariable();

        var->type = GetType(ct.value);
        var->size = GetTypeSize(ct.value);
        var->localPos = GetLocalPos(scope);

        NextToken();
        if(GetLocalVariable(scope, ct.value)!=NULL)
        {
            ErrorMsg("Redefinition of variable: " + ct.value);
        }
        var->name = ct.value;
        scope.variables.push_back(var);
        //If only initialisation is permitted (structures etc)
        if(init)
        {
            NextToken();
            //If semicolon required at the end
            if(expectEnd)
            {
                if (!AcceptToken(SilentTokenType::Semicolon))
                    ErrorMsg("Expected \";\" at the end of declaration");
                NextToken();
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
            if(PeakToken().type == SilentTokenType::Semicolon)
            {
                var->initialised = false;
                NextToken();
                #if DEBUG
                std::cout << "Finished parsing var " << var->name << "\n\n";
                #endif
                NextToken();
                return var;
            }
            else
            {
                SilentStatement* statement = ParseStatement(scope);
                scope.statements.push_back(statement);

                var->initialised = true;
                #if DEBUG
                    std::cout << "Syntax tree:\n";
                    SilentPrintTree(statement->expression);
                    std::cout << "Finished parsing var " << var->name << "\n\n";
                #endif
                return var;
            }
        }
        return var;
    }

    SilentStructure* SilentParser::ParseStruct(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Parsing struct\n";
        #endif

        SilentStructure* structure = new SilentStructure();
        structure->variables = new SilentLocalScope();
        
        //Get structure name
        NextToken();
        if(IsValidType(ct.value))
            ErrorMsg("Identifier " + ct.value + " already in use");
        else structure->name = ct.value;
        NextToken();
        ExpectToken(SilentTokenType::OpenScope, "Expected struct declaration");
        NextToken();

        //Parse structure body
        while(!AcceptToken(SilentTokenType::CloseScope))
        {
            if(GetType(ct.value).primitive != SilentPrimitives::undefined)
            {
                SilentStatement* statement = new SilentStatement();
                statement->type = SilentStatementType::VarInit;

                SilentVariable* var = ParseVariable(
                    *structure->variables, true, true
                );
                //structure->variables->variables.push_back(var);
                structure->variables->statements.push_back(statement);
                structure->size += var->size;
                #if DEBUG
                    std::cout << "struct size " << structure->size << "\n";
                #endif
            }
            else ErrorMsg("Use of non-existing type");
        }

        NextToken();
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

    SilentLocalScope* SilentParser::ParseParameters()
    {
        #if DEBUG
        std::cout << "Parsing parameters\n";
        #endif

        SilentLocalScope* parameters = new SilentLocalScope();
        while(ct.type != SilentTokenType::CloseParam)
        {
            //SilentStatement* statement = new SilentStatement();
            //statement->type = SilentStatementType::VarInit;
            ParseVariable(*parameters,true,false);
            //parameters->statements.push_back(statement);
            if(ct.value == ",") {NextToken(); continue;}
            else if(ct.type == SilentTokenType::CloseParam) break;
            else ErrorMsg("Got invalid token whilst parsing parameters");
        }
        NextToken();

        #if DEBUG
        std::cout << "finished parsing parameters\n\n";
        #endif
        
        return parameters;
    }

    
    void SilentParser::ParseIfStatement(SilentLocalScope &scope)
    {
        #if DEBUG
        std::cout << "Parsing if statement\n";
        #endif

        SilentIfStatement* ifStatement = new SilentIfStatement();
        SilentStatement* statement = new SilentStatement();
        statement->type = SilentStatementType::If;
        statement->ifStatement = ifStatement;
        scope.statements.push_back(statement);

        ifStatement->expression = ParseExpression();
        ifStatement->scope = new SilentLocalScope();
        ifStatement->scope->hasParent = true;
        ifStatement->scope->scopeParent = &scope;
        ParseLocalScope(*ifStatement->scope);

        #if DEBUG
        std::cout << "Finished parsing local scope\n\n";
        #endif
    }

    void SilentParser::ParseLocalScope(SilentLocalScope &scope)
    {
        #if DEBUG
        std::cout << "Parsing local scope\n";
        #endif
        
        while(!AcceptToken(SilentTokenType::CloseScope))
        {
            switch(ct.type)
            {
                case SilentTokenType::Primitive:
                {
                    SilentStatement* statement = new SilentStatement();
                    statement->type = SilentStatementType::VarInit;
                    scope.statements.push_back(statement);

                    SilentVariable* var = 
                        ParseVariable(scope, false, true);

                    statement->variable = var;
                    //localScope->variables.push_back(var);
                }
                break;

                case SilentTokenType::Identifier:
                    if(IsValidType(ct.value))
                    {
                        SilentStatement* statement = new SilentStatement();
                        statement->type = SilentStatementType::VarInit;

                        SilentVariable* var = 
                            ParseVariable(scope, false, true);

                        statement->variable = var;
                        scope.statements.push_back(statement);
                        //localScope->variables.push_back(var);
                    }
                    else
                    {
                        SilentVariable* var = GetLocalVariable(
                            scope, ct.value.data());
                        if(var == NULL) ErrorMsg("Use of invalid type");
                        //NextToken();
                        printf("Token %s\n", ct.value.data());
                        SilentStatement* statement = ParseStatement(scope);
                        // statement->type = SilentStatementType::Expression;
                        // statement->expression = ParseExpression(); 
                        scope.statements.push_back(statement);
                    }
                break;

                default:
                    ErrorMsg("Unexpected token in the local scope");
                    NextToken();NextToken();
                break;

                //Add other statements
            }
        }
        //nextToken();
        #if DEBUG
        std::cout << "Finished parsing local scope\n\n";
        #endif
    }

    SilentFunction* SilentParser::ParseFunction(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Parsing function\n";
        #endif
        //Create function
        SilentFunction* function = new SilentFunction();

        scope.functions.push_back(function);

        //Get function return type
        NextToken();
        function->returnType = GetType(ct.value);

        //Get function name
        NextToken();
        if(GetLocalFunction(ct.value) != NULL)
            ErrorMsg("Identifier already in use locally");
        else function->name = ct.value;

        //Get function parameters
        NextToken();
        if(!AcceptToken(SilentTokenType::OpenParam))
            ErrorMsg("Expected \"(\" for parameter declaration");
        NextToken();
        //function->parameters = 
        function->scope = ParseParameters();
        if(function->scope->variables.size() > 0)
        {
            function->parameterCount = function->scope->variables.size();
            SilentVariable* lastParam = function->scope->variables.back();
            function->parameterSize = lastParam->localPos + lastParam->size;
        }
        else
        {
            function->parameterCount = 0;
            function->parameterSize = 0;
        }
        //function->scope->variables.assign(function->parameters);
        //function->scope->variables = function->parameters->variables;
        //Parse function scope
        if(!AcceptToken(SilentTokenType::OpenScope))
        {
            function->initialised = false;
            if(!AcceptToken(SilentTokenType::Semicolon))
            {
                ErrorMsg("Expected \";\" at the end of uninitialised function");
            }
            //nextToken();
        }
        else
        {
            function->initialised = true;
            NextToken();
            function->scope->hasParent = false;
            ParseLocalScope(*function->scope);
            std::cout << "v: "<<function->scope->variables.size()<<"\n";
            std::cout << "s: "<<function->scope->statements.size()<<"\n";
        }

        #if DEBUG
            std::cout << "Declared function " << function->name.data() << "\n";
        #endif

        NextToken();
        #if DEBUG
        std::cout << "Finished parsing function\n\n";
        #endif
        return function;
    }

    SilentNamespace* SilentParser::ParseNamespace(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Parsing namespace\n";
        #endif

        SilentNamespace* newNamespace = new SilentNamespace();
        accessibleNamespaces.push_back(newNamespace);

        //Get namespace name
        NextToken();
        ExpectToken(SilentTokenType::Identifier, "Expected scope name");
        newNamespace->name = ct.value;
        for(SilentNamespace* name : scope.namespaces)
        {
            if(name->name == newNamespace->name) 
                ErrorMsg("Conflicting namespaces");
        }
        NextToken();
        ExpectToken(SilentTokenType::OpenScope, "Expected scope declaration");
        NextToken();

        while(!AcceptToken(SilentTokenType::CloseScope))
        {
            switch(ct.type)
            {
                case SilentTokenType::Namespace:
                    newNamespace->namespaces.push_back(
                        ParseNamespace(*newNamespace)
                    );
                    newNamespace->namespaces.back()->parent = newNamespace;
                break;

                case SilentTokenType::Struct:
                    newNamespace->types.push_back(
                        ParseStruct(*newNamespace)
                    );
                break;

                case SilentTokenType::Function:
                    ParseFunction(*newNamespace);
                break;

                case SilentTokenType::Identifier:
                case SilentTokenType::Primitive:
                    if(IsValidType(ct.value))
                    {
                        SilentVariable* var = ParseVariable(
                            *globalNamespace->globals, true, true);
                        newNamespace->globals->variables.push_back(var);
                        globalVarPointer += var->size;
                    }
                    else
                    {
                        ErrorMsg("Unexpected token in namespace scope");
                        NextToken();
                    }
                break;

                default:
                    ErrorMsg("Unexpected token in namespace scope");
                break;
            }
        }
        NextToken();
        accessibleNamespaces.pop_back();

        #if DEBUG
        std::cout<<"Finished parsing namespace: "<<newNamespace->name<<"\n\n";
        #endif
        return newNamespace;
    }
}