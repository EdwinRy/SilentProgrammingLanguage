#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
#include "SilentDebugger.hpp"
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
#define DEBUG 1

namespace Silent
{
    SilentParser::SilentParser(){this->globalNamespace = new SilentNamespace();}
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
        globalNamespace->globals->namespaceParent = globalNamespace;
        globalNamespace->globals->usesScopeParent = false;
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
                    globalNamespace->functions.push_back(
                        ParseFunction(*globalNamespace)
                    );
                break;

                case SilentTokenType::Identifier:
                case SilentTokenType::Primitive:
                    globalNamespace->globals->variables.push_back(
                        ParseVariable(*globalNamespace->globals, false, true)
                    );
                break;

                default:
                    ErrorMsg("Unexpected token in the global scope");
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

    uint64 SilentParser::GetLocalPos(SilentLocalScope &scope)
    {
        //uint64 scopeSize = scope.variables.size();
        uint64 localPos = 0;
        for(auto& var : scope.variables) localPos += var->size;
        return localPos;
    }

    void SilentParser::NextToken()
    {
        tokenCursor++;
        if(tokenCursor == tokensPtr->size()) tokenCursor--;
        ct = (*tokensPtr)[tokenCursor];
    }

    bool SilentParser::AcceptToken(SilentTokenType type)
    {
        if(ct.type == type) return true;
        return false;
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

        if(AcceptToken(SilentTokenType::Number))
        {
            SilentOperand* operand = new SilentOperand();
            operand->type = SilentOperandType::Number;
            operand->token = new SilentToken;
            *(operand->token) = ct;
            NextToken();
            return operand;
        }
        else if(AcceptToken(SilentTokenType::Identifier))
        {
            SilentOperand* operand = new SilentOperand();
            operand->type = SilentOperandType::Number;
            NextToken();
            return operand;
        }
        else if(AcceptToken(SilentTokenType::OpenParam))
        {
            NextToken();
            SilentOperand* operand = ParseExpression();
            ExpectToken(SilentTokenType::CloseScope, "Expected token ')'");
            NextToken();
            return operand;
        }
        ErrorMsg("Syntax Error");
        NextToken();
        return NULL;
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
                NextToken();
                operand->right = ParseFactor();
                temp = operand;
                operand = new SilentOperand();
                operand->left = temp;
            }
            else if(AcceptToken(SilentTokenType::Divide))
            {
                operand->type = SilentOperandType::Divide;
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
    SilentOperand* SilentParser::ParseExpression()
    {
        if(AcceptToken(SilentTokenType::Add) ||
            AcceptToken(SilentTokenType::Subtract)
        )
        {
            NextToken();
        }

        SilentOperand* operand = new SilentOperand();
        SilentOperand* temp;
        operand->left = ParseTerm();

        while(true)
        {
            if(AcceptToken(SilentTokenType::Add))
            {
                operand->type = SilentOperandType::Add;
                NextToken();
                operand->right = ParseTerm();
                temp = operand;
                operand = new SilentOperand();
                operand->left = temp;
                
            }
            else if(AcceptToken(SilentTokenType::Subtract))
            {
                operand->type = SilentOperandType::Subtract;
                NextToken();
                operand->right = ParseTerm();
                temp = operand;
                operand = new SilentOperand();
                operand->left = temp;
            }
            else break;
        }
        temp = operand->left;
        return temp;
    }

    // SilentStatement* SilentParser::ParseStatement(SilentLocalScope &scope)
    // {

    // }

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
        var->name = ct.value;
        NextToken();

        //If only initialisation is permitted (structures etc)
        if(init)
        {
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
            //Parse statement
            if(AcceptToken(SilentTokenType::Assign))
            {    
                NextToken();
                var->expresion = new SilentOperand();
                var->expresion->type = SilentOperandType::Assign;
                var->expresion->left = new SilentOperand();
                var->expresion->left->type = SilentOperandType::Variable;
                var->expresion->left->variable = var;
                var->expresion->right = ParseExpression();
                if(ExpectToken(SilentTokenType::Semicolon,
                    "Expected \";\" at the end of expression")
                )
                {
                    var->initialised = true;
                    NextToken();
                    #if DEBUG
                        std::cout << "Syntax tree:\n";
                        SilentPrintTree(var->expresion);
                        std::cout << "Finished parsing var " << var->name << "\n\n";
                    #endif
                    return var;
                }
                NextToken();
            }
            //End declaration
            else if(AcceptToken(SilentTokenType::Semicolon))
            {
                var->initialised = false;
                NextToken();
                #if DEBUG
                std::cout << "Finished parsing var " << var->name << "\n\n";
                #endif
                return var;
            }
            else ErrorMsg("Invalid token following an expression");
            #if DEBUG
            std::cout << "Finished parsing var " << var->name << "\n\n";
            #endif
            return var;
        }
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
                structure->variables->variables.push_back(var);
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

    SilentLocalScope* SilentParser::ParseParameters(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Parsing parameters\n";
        #endif

        SilentLocalScope* parameters = new SilentLocalScope();
        while(ct.type != SilentTokenType::CloseParam)
        {
            SilentStatement* statement = new SilentStatement();
            statement->type = SilentStatementType::VarInit;
            parameters->variables.push_back(
                ParseVariable(*parameters,true,false));
            parameters->statements.push_back(statement);
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

    SilentLocalScope* SilentParser::ParseLocalScope(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Parsing local scope\n";
        #endif
        SilentLocalScope* localScope = new SilentLocalScope();
        localScope->usesScopeParent = false;
        localScope->namespaceParent = &scope;
        
        while(!AcceptToken(SilentTokenType::CloseScope))
        {
            switch(ct.type)
            {
                case SilentTokenType::Identifier:
                case SilentTokenType::Primitive:
                    {
                        SilentVariable* var = 
                            ParseVariable(*localScope, false, true);

                        SilentStatement* statement = new SilentStatement();
                        statement->type = SilentStatementType::VarInit;
                        statement->variable = var;
                        localScope->variables.push_back(var);
                        if(var->initialised) 
                            localScope->statements.push_back(statement);
                    }
                break;

                default:
                    ErrorMsg("Unexpected token in the local scope");
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

    SilentFunction* SilentParser::ParseFunction(SilentNamespace& scope)
    {
        #if DEBUG
        std::cout << "Parsing function\n";
        #endif
        //Create function
        SilentFunction* function = new SilentFunction();

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
        function->parameters = ParseParameters(scope);

        //Parse function scope
        if(!AcceptToken(SilentTokenType::OpenScope))
        {
            function->initialised = false;
            function->scope = new SilentLocalScope();
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
            function->scope = ParseLocalScope(scope);
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
                    newNamespace->functions.push_back(
                        ParseFunction(*newNamespace)
                    );
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