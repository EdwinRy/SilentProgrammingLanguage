#include "SilentStructures.hpp"
#include "SilentParser.hpp"
#include <iostream>
#include <stdlib.h>

#define DEBUG_ENABLED 1
#define ERROR(args...) printf(args);
#if DEBUG_ENABLED
#define DEBUG(args...) printf(args);
#else
#define DEBUG(args...) 
#endif

namespace Silent::Structures
{
    typedef long long int int64;
    bool Variable::Parse(Parser &parser, LocalScope &scope, bool isParam)
    {
        DEBUG("Parsing variable\n")

        //Get variable information
        this->type = parser.GetType();
        this->size = this->type.size;
        this->localPos = scope.GetLocalPos();

        //Get variable identifier
        std::string id = parser.NextToken().value;
        if(scope.GetVariable(id) != NULL)
            parser.ErrorMsg("Redefinition of a variable: " + id);
        this->identifier = id;

        //Parse structure member or a parameter
        if(scope.GetParentType() == NodeType::Structure || isParam)
        {
            parser.NextToken();
            if(!isParam)
                //Enforce semicolon at the end if it's not a parameter
                parser.ExpectToken(TokenType::Semicolon, 
                    "Expected \";\" at the end of "
                    "declaration of a structure member");
            //else parser.NextToken();
        }
        else
        {
            if(parser.PeakToken().type == TokenType::Semicolon)
            {
                this->initialised = false;
                parser.NextToken(); //ct = ;
                parser.NextToken();
            }
            else
            {
                this->initialised = true;
                Statement* statement = new Statement();
                scope.GetStatements()->push_back(statement);
                statement->Parse(parser, scope);
            }
        }

        DEBUG((char*)("Declared variable %s\n of size %llu\n "
            "at local position %llu\n"), id.data(), this->size, this->localPos);
        return true;
    }

    bool Structure::Parse(Parser &parser, Namespace& scope)
    {
        DEBUG("Parsing struct\n")

        //Check whether the struct has been defined within parent namespace
        std::string id = parser.NextToken().value;
        if(scope.ContainsStructure(id))
        {
            DEBUG("Structure %s already declared in the namespace %s\n", 
                id.data(), scope.GetId().data());
            return false;
        }

        //Get structure identifier
        this->identifier = id;
        parser.NextToken();
        parser.ExpectNextToken(
            TokenType::OpenScope, "Expected struct declaration");

        //Parse struct body
        this->members = new LocalScope();
        this->members->parent = new Node();
        this->members->parent->type = NodeType::Structure;
        this->members->parent->data.structure = this;

        //Parse until the closing scope token is found
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            //If member's type is not undefined parse member's declaration
            if(parser.GetType().type->type != NodeType::Undefined)
            {
                Statement* statement = new Statement();
                statement->val = new Node();
                statement->type = StatementType::VarInit;

                Variable* var = new Variable();
                statement->val->type = NodeType::Variable;
                statement->val->data.variable = var;
                this->members->GetVariables()->push_back(var);
                var->Parse(parser, *members, false);
                this->size += var->GetSize();
            } 
            else parser.ErrorMsg("Use of non-existing type");
        }
        parser.NextToken();
        DEBUG("Struct size: %llu\n", this->size);
        DEBUG("Finished parsing struct \n")
        return true;
    }

    //Parse local scope
    bool LocalScope::Parse(Parser &parser)
    {
        DEBUG("Parsing local scope\n")
        //Parse until scope closing token is found
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            switch(parser.GetToken().type)
            {
                //Parse variable as reference to datatype
                case TokenType::Reference:
                {
                    parser.NextToken();
                    Statement* statement = new Statement();
                    statement->type = StatementType::VarInit;
                    this->statements.push_back(statement);
                    statement->val = new Node();
                    statement->val->type = NodeType::Variable;
                    Variable* var = new Variable();
                    statement->val->data.variable = var;
                    this->variables.push_back(var);
                    var->Parse(parser, *this, false);
                    this->varSize += var->GetSize();
                    var->isReference = true;
                }
                break;

                //Handle explicit deletion of a reference
                case TokenType::Delete:
                {
                    //Setup statement
                    Statement* statement = new Statement();
                    statement->type = StatementType::Delete;
                    //Add statement to this scope's statement list
                    this->statements.push_back(statement);
                    //Create new node for the statement
                    statement->val = new Node();
                    statement->val->type = NodeType::Variable;
                    //Get variable to delete from this scope
                    statement->val->data.variable = 
                        this->GetVariable(parser.NextToken().value);

                    //Skip through semicolon for syntactic consistency
                    parser.NextToken(); parser.NextToken();
                }
                break;
                
                //Parse variable of primitive type
                case TokenType::Primitive:
                {
                    //Setup statement
                    Statement* statement = new Statement();
                    statement->type = StatementType::VarInit;
                    //Add statement to this scope's statement list
                    this->statements.push_back(statement);
                    //Create new node for the statement
                    statement->val = new Node();
                    statement->val->type = NodeType::Variable;
                    //Parse new variable declaration
                    Variable* var = new Variable();
                    this->variables.push_back(var);
                    var->Parse(parser, *this, false);
                    var->isReference = false;
                    this->varSize += var->GetSize();
                    //Add variable to statement
                    statement->val->data.variable = var;
                }
                break;
                
                case TokenType::Identifier:
                {
                    //If it's a definition
                    if(parser.GetType().type->type != NodeType::Undefined)
                    {
                        //Setup statement
                        Statement* statement = new Statement();
                        statement->type = StatementType::VarInit;
                        //Add statement to this scope's statement list
                        this->statements.push_back(statement);
                        //Create new node for the statement
                        statement->val = new Node();
                        statement->val->type = NodeType::Variable;
                        //Parse new variable declaration
                        Variable* var = new Variable();
                        var->Parse(parser, *this, false);
                        var->isReference = false;
                        //Add variable to statement
                        statement->val->data.variable = var;
                    }
                    //If it's a statement starting with an identifier
                    else
                    {
                        Variable* var = 
                           parser.GetVariable(*this,parser.GetToken().value);

                        //If it's not a valid variable (function call)
                        if(var == NULL)
                        {
                            DEBUG("FUNCTION CALL\n");
                            Statement* statement = new Statement();
                            statement->type = StatementType::FunctionCall;
                            statement->val = new Node();
                            statement->val->type = NodeType::FunctionCall;
                            this->statements.push_back(statement);
                            statement->val->data.functionCall = 
                                Operand::ParseFunctionCall(parser, *this);
                        }
                        else
                        {
                            DEBUG("VARIABLE STATEMENT\n");
                            Statement* statement = new Statement();
                            statement->Parse(parser, *this);
                            //statement->type = StatementType::Expression;
                            this->statements.push_back(statement);
                            //statement->val = new Node();
                            //statement->val->type = NodeType::Operand;
                        }
                    }
                }
                break;
                
                //Parse if statement
                case TokenType::If:
                {
                    Statement* statement = new Statement();
                    statement->type = StatementType::If;
                    statement->val = new Node();
                    statement->val->type = NodeType::IfStatement;
                    IfStatement* ifStatement = new IfStatement();
                    statement->val->data.ifStatement = ifStatement;
                    statement->parentScope = this;
                    ifStatement->Parse(parser, *this);
                    this->statements.push_back(statement);
                }
                break;
                
                //Parse else statement
                case TokenType::Else:
                {
                    //Set the last if statement to have a next if
                    this->statements.back()->val->data.ifStatement->hasNext = 
                        true;
                    parser.NextToken();
                    //If it's an else if statement
                    if(parser.GetToken().type == TokenType::If)
                    {
                        Statement* statement = new Statement();
                        statement->type = StatementType::If;
                        statement->val = new Node();
                        statement->val->type = NodeType::IfStatement;
                        IfStatement* ifStatement = new IfStatement();
                        statement->val->data.ifStatement = ifStatement;
                        statement->parentScope = this;
                        ifStatement->Parse(parser, *this);
                        this->statements.push_back(statement);
                    }
                    //If it's an else on its own
                    else if(parser.GetToken().type == TokenType::OpenScope)
                    {
                        DEBUG("Parsing else statement\n");
                        IfStatement* ifStatement = new IfStatement();
                        Statement* statement = new Statement();
                        statement->type = StatementType::If;
                        statement->val = new Node();
                        statement->val->type = NodeType::IfStatement;
                        statement->val->data.ifStatement = ifStatement;
                        this->statements.push_back(statement);
                        ifStatement->scope = new LocalScope();
                        ifStatement->scope->parent = new Node();
                        ifStatement->scope->parent->type = NodeType::LocalScope;
                        ifStatement->scope->parent->data.localScope = this;
                        parser.NextToken();
                        ifStatement->scope->Parse(parser);
                        DEBUG("Finished parsing else statement\n");
                    }
                }
                break;
                
                //Parse return statement
                case TokenType::Return:
                {
                    DEBUG("Parsing return statement\n");
                    //If an expression follows the return statement
                    if(parser.NextToken().type != TokenType::Semicolon)
                    {
                        DEBUG("Parsing return statement with expression\n");
                        //Set expected type to return type
                        parser.currentType = 
                            parser.currentFunction->GetReturnType();
                    
                        //Parse return expression
                        Statement* returnExpression = new Statement();
                        returnExpression->type = StatementType::Expression;
                        returnExpression->val = new Node();
                        Operand* op = new Operand();
                        returnExpression->val->type = NodeType::Operand;
                        returnExpression->val->data.operand = op;
                        op->Parse(parser, *this); 
                        this->statements.push_back(returnExpression);
                        //if(parser.GetToken().type != TokenType::Semicolon)
                        //    parser.ErrorMsg("Expected end of statement");
                    }
                    parser.NextToken();
                    //Add return statement
                    Statement* statement = new Statement();
                    statement->type = StatementType::Return;
                    statement->val = new Node();
                    statement->val->type = NodeType::DataType;
                    statement->val->data.dataType = 
                        new DataType(parser.currentType);
                    statement->parentScope = this;
                    this->statements.push_back(statement);
                }
                break;
                
                //Handle unknown token in the local scope
                default: 
                {
                    parser.ErrorMsg("Unexpected token in the local scope");
                    parser.NextToken(); parser.NextToken();
                }
                break;
            }
        }
        //parser.ExpectToken(TokenType::CloseScope, "Expected closing scope\n");
        if(parser.GetToken().type != TokenType::CloseScope)
            parser.ErrorMsg("Expected closing scope");
        parser.NextToken();
        DEBUG("Finished parsing local scope\n")
        return true;
    }

    bool LocalScope::ParseParameters(Parser &parser, Function &function)
    {
        DEBUG("Parsing parameters\n")
        this->parent = new Node();
        this->parent->type = NodeType::Function;
        this->parent->data.function = &function;

        while(parser.GetToken().type != TokenType::CloseParam)
        {
            if(parser.GetToken().type == TokenType::Reference)
            {
                parser.NextToken();
                Variable* var = new Variable();
                this->variables.push_back(var);
                var->Parse(parser, *this, true);
                var->isReference = true;
                this->varSize += 8;
            }
            else
            {
                Variable* var = new Variable();
                this->variables.push_back(var);
                var->Parse(parser, *this, true);
                var->isReference = false;
                this->varSize += var->GetSize();
            }
            if(parser.GetToken().value == ",") {parser.NextToken(); continue;}
            else if(parser.GetToken().type == TokenType::CloseParam) break;
            else parser.ErrorMsg("Got invalid token whilst parsing parameters");
        }
        parser.NextToken();
        return true;
    }

    Variable* LocalScope::GetVariable(std::string id)
    {
        for(Variable* var : this->variables){if(var->GetId() == id) return var;}
        return NULL;
    }

    bool Function::Parse(Parser &parser, Namespace &scope)
    {
        DEBUG("Parsing function\n")

        parser.currentFunction = this;

        //Get return type
        this->returnType = parser.GetType(parser.NextToken().value);

        //Check whether function has already been declared in the namespace
        std::string id = parser.NextToken().value;
        if(scope.ContainsFunction(id))
        {
            DEBUG("Function %s already declared in the namespace %s\n", 
                id.data(), scope.GetId().data())
            return false;
        }

        //Assign name to the function
        this->identifier = id;
        DEBUG("Got function identifier: %s\n", id.data());

        //Parse function parameters
        parser.ExpectNextToken(TokenType::OpenParam, "Expected token \"(\"");
        this->scope = new LocalScope();
        this->scope->ParseParameters(parser, *this);

        //Get information about parameters
        this->parameterCount = this->scope->GetVariables()->size();
        this->parameterSize = this->scope->GetLocalPos();

        //Parse local scope
        if(parser.GetToken().type == TokenType::OpenScope)
        {
            parser.NextToken();
            this->initialised = true;
            this->scope->Parse(parser);
        }
        else if(parser.GetToken().type == TokenType::Semicolon)
        {
            this->initialised = false;
            parser.NextToken();
        }
        else
        {
            parser.NextToken();
            parser.ErrorMsg("Unexpected token in function declaration");
        }
        
        DEBUG("Finished parsing function %s\n", id.data());
        return true;
    }

    bool Operand::Parse(Parser &parser, LocalScope &scope)
    {
        //Parse expression
        Operand* op = ParseExpression(parser, scope);

        //Copy data to this instance
        value = op->value;
        expressionType = op->expressionType;
        operandType = op->operandType;
        left = op->GetLeft();
        right = op->GetRight();

        //Clean up old object
        // delete op;

        return true;
    }

    FunctionCall* Operand::ParseFunctionCall(Parser &parser, LocalScope &scope)
    {
        //Setup function call and get the called function reference
        FunctionCall* call = new FunctionCall();
        call->function = parser.GetFunction();
        parser.NextToken(); parser.NextToken();

        uint64 argPtr = 0; //Number of arguments

        //Save previous expected data type
        DataType oldType = parser.currentType;

        //Parse arguments until ran out of parameters or met with ")"
        while(parser.GetToken().type != TokenType::CloseParam || 
            argPtr < call->function->GetParameterCount()
        )
        {
            //Assign expected type to the parameter's type
            parser.currentType = 
                call->function->GetVariables()->at(argPtr)->GetType();
            //Add expression as function call's argument
            Operand* op = new Operand();
            call->arguments.push_back(op);
            op->Parse(parser, scope);
            argPtr++;
            //If there is a next argument skip the comma
            if(parser.GetToken().type == TokenType::Comma) parser.NextToken();
            //Otherwise, stop parsing arguments
            else break;
        }

        //Check whether the number of arguments matches to parameters
        if(argPtr != call->function->GetParameterCount())
            parser.ErrorMsg(
                "Insufficient number arguments provided to call function" +
                call->function->GetId());

        //Restore old expected data type
        parser.currentType = oldType;

        //Expect semicolon at the end
        parser.ExpectToken(
            TokenType::Semicolon, "Expected end of statement");

        return call;
    }

    Operand* Operand::ParseFactor(Parser &parser, LocalScope &scope)
    {
        switch(parser.GetToken().type)
        {
            case TokenType::Number:
            {
                Operand* op = new Operand();
                op->expressionType = ExpressionType::Data;
                op->operandType = OperandType::Number;
                op->value = new Node();
                op->value->type = NodeType::Value;
                op->value->data.value = new Value();
                op->value->data.value->data = parser.GetToken().value;
                op->value->data.value->type = parser.currentType;
                parser.NextToken();
                return op;
            }
            break;

            case TokenType::Identifier:
            {
                Operand* op = new Operand();
                //Parse function call
                if(parser.PeakToken().type == TokenType::OpenParam)
                {
                    op->operandType = OperandType::FunctionCall;
                    op->expressionType = ExpressionType::Memory;
                    op->value = new Node();
                    op->value->type = NodeType::FunctionCall;
                    op->value->data.functionCall = 
                        ParseFunctionCall(parser,scope);
                }
                //Parse statement referencing variable
                else
                {
                    op->operandType = OperandType::Variable;
                    op->expressionType = ExpressionType::Data;
                    op->value = new Node();
                    op->value->type = NodeType::Variable;
                    op->value->data.variable = parser.GetVariable(scope);
                    if(op->value->data.variable == NULL)
                        parser.ErrorMsg("Use of undeclared identifier");
                    parser.NextToken();
                }
                return op;
            }
            break;

            case TokenType::OpenParam:
            {
                parser.NextToken();
                Operand* op = new Operand();
                op->Parse(parser, scope);
                parser.ExpectNextToken(
                    TokenType::CloseParam, "Expected token ')'");
                return op;
            }
            break;

            default:
            {
                parser.ErrorMsg("Syntax error whilst parsing factor");
                parser.NextToken();
                return NULL;
            }
            break;
        }
        return NULL;
    }

    Operand* Operand::ParseTerm(Parser &parser, LocalScope &scope)
    {
        Operand* op = new Operand();
        Operand* temp = new Operand();

        op->left = ParseFactor(parser, scope);

        for(;;)
        {
            if(parser.GetToken().type == TokenType::Multiply)
            {
                op->operandType = OperandType::Multiply;
                op->expressionType = ExpressionType::Arithmetic;
                parser.NextToken();
                op->right = ParseFactor(parser, scope);
                temp = op;
                op = new Operand();
                op->left = temp;
            }

            else if(parser.GetToken().type == TokenType::Divide)
            {
                op->operandType = OperandType::Divide;
                op->expressionType = ExpressionType::Arithmetic;
                parser.NextToken();
                op->right = ParseFactor(parser, scope);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            else break;
        }
        return op->left;
    }

    Operand* Operand::ParseSum(Parser &parser, LocalScope &scope)
    {
        switch(parser.GetToken().type)
        {
            case TokenType::Add:
            case TokenType::Subtract:
                parser.NextToken();
            break;
            default: break;
        }

        Operand* op = new Operand();
        Operand* temp = new Operand();

        op->left = ParseTerm(parser, scope);

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::Add:
                {
                    op->operandType = OperandType::Add;
                    op->expressionType = ExpressionType::Arithmetic;
                    parser.NextToken();
                    op->right = ParseTerm(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::Subtract:
                {
                    op->operandType = OperandType::Subtract;
                    op->expressionType = ExpressionType::Arithmetic;
                    parser.NextToken();
                    op->right = ParseTerm(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                default: parsingExpression = false; break;
            }
        }
        return op->left;
    }

    Operand* Operand::ParseLogic(Parser &parser, LocalScope &scope)
    {
        switch(parser.GetToken().type)
        {
            case TokenType::And:
            case TokenType::Or:
            case TokenType::Not:
                parser.NextToken();
            break;
            default: break;
        }

        Operand* op = new Operand();
        Operand* temp = new Operand();

        op->left = ParseSum(parser, scope);

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::And:
                {
                    op->operandType = OperandType::And;
                    op->expressionType = ExpressionType::Logical;
                    parser.NextToken();
                    op->right = ParseSum(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::Or:
                {
                    op->operandType = OperandType::Or;
                    op->expressionType = ExpressionType::Logical;
                    parser.NextToken();
                    op->right = ParseSum(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::Xor:
                {
                    op->operandType = OperandType::Xor;
                    op->expressionType = ExpressionType::Logical;
                    parser.NextToken();
                    op->right = ParseSum(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::Not:
                {
                    op->operandType = OperandType::Not;
                    op->expressionType = ExpressionType::Logical;
                    parser.NextToken();
                    op->left = ParseSum(parser, scope);
                }
                break;

                default: parsingExpression = false; break;
            }
        }
        return op->left;
    }

    Operand* Operand::ParseComparison(Parser &parser, LocalScope &scope)
    {
        switch(parser.GetToken().type)
        {
            case TokenType::Equal:
            case TokenType::NotEqual:
            case TokenType::Larger:
            case TokenType::LargerOrEqual:
            case TokenType::Smaller:
            case TokenType::SmallerOrEqual:
                parser.NextToken();
            break;
            default: break;
        }

        Operand* op = new Operand();
        Operand* temp = new Operand();

        op->left = ParseLogic(parser, scope);

        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::Equal:
                {
                    op->operandType = OperandType::Equal;
                    op->expressionType = ExpressionType::Comparison;
                    parser.NextToken();
                    op->right = ParseLogic(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::NotEqual:
                {
                    op->operandType = OperandType::NotEqual;
                    op->expressionType = ExpressionType::Comparison;
                    parser.NextToken();
                    op->right = ParseLogic(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::Larger:
                {
                    op->operandType = OperandType::Larger;
                    op->expressionType = ExpressionType::Comparison;
                    parser.NextToken();
                    op->right = ParseLogic(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::LargerOrEqual:
                {
                    op->operandType = OperandType::LargerOrEqual;
                    op->expressionType = ExpressionType::Comparison;
                    parser.NextToken();
                    op->right = ParseLogic(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::Smaller:
                {
                    op->operandType = OperandType::Smaller;
                    op->expressionType = ExpressionType::Comparison;
                    parser.NextToken();
                    op->right = ParseLogic(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;
                
                case TokenType::SmallerOrEqual:
                {
                    op->operandType = OperandType::SmallerOrEqual;
                    op->expressionType = ExpressionType::Comparison;
                    parser.NextToken();
                    op->right = ParseLogic(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                default: parsingExpression = false; break;
            }
        }
        return op->left;
    }

    Operand* Operand::ParseExpression(Parser &parser, LocalScope &scope)
    {
        if(parser.GetToken().type == TokenType::Assign) parser.NextToken();
        switch(parser.GetToken().type)
        {
            case TokenType::Assign:
            case TokenType::AddAssign:
            case TokenType::SubtractAssign:
            case TokenType::MultiplyAssign:
            case TokenType::DivideAssign:
                parser.NextToken();
            break;
            default: break;
        }

        Operand* op = new Operand();
        Operand* temp;

        op->left = ParseComparison(parser, scope);
        
        bool parsingExpression = true;
        while(parsingExpression)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::Assign:
                    parser.currentType = 
                        op->left->value->data.variable->GetType();
                    op->operandType = OperandType::Assign;
                    op->expressionType = ExpressionType::Memory;
                    parser.NextToken();
                    op->right = ParseComparison(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                break;

                case TokenType::AddAssign:
                {
                    parser.currentType = 
                        op->left->value->data.variable->GetType();
                    op->operandType = OperandType::Assign;
                    op->expressionType = ExpressionType::Memory;
                    Operand* addOp = new Operand();
                    op->right = addOp;
                    addOp->operandType = OperandType::Add;
                    addOp->expressionType = ExpressionType::Arithmetic;
                    addOp->left = op->left;
                    parser.NextToken();
                    addOp->right = ParseComparison(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::SubtractAssign:
                {
                    parser.currentType = 
                        op->left->value->data.variable->GetType();
                    op->operandType = OperandType::Assign;
                    op->expressionType = ExpressionType::Memory;
                    Operand* subOp = new Operand();
                    op->right = subOp;
                    subOp->operandType = OperandType::Subtract;
                    subOp->expressionType = ExpressionType::Arithmetic;
                    subOp->left = op->left;
                    parser.NextToken();
                    subOp->right = ParseComparison(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::MultiplyAssign:
                {
                    parser.currentType = 
                        op->left->value->data.variable->GetType();
                    op->operandType = OperandType::Assign;
                    op->expressionType = ExpressionType::Memory;
                    Operand* mulOp = new Operand();
                    op->right = mulOp;
                    mulOp->operandType = OperandType::Multiply;
                    mulOp->expressionType = ExpressionType::Arithmetic;
                    mulOp->left = op->left;
                    parser.NextToken();
                    mulOp->right = ParseComparison(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                case TokenType::DivideAssign:
                {
                    parser.currentType = 
                        op->left->value->data.variable->GetType();
                    op->operandType = OperandType::Assign;
                    op->expressionType = ExpressionType::Memory;
                    Operand* divOp = new Operand();
                    op->right = divOp;
                    divOp->operandType = OperandType::Divide;
                    divOp->expressionType = ExpressionType::Arithmetic;
                    divOp->left = op->left;
                    parser.NextToken();
                    divOp->right = ParseComparison(parser, scope);
                    temp = op;
                    op = new Operand();
                    op->left = temp;
                }
                break;

                default: parsingExpression = false; break;
            }
        }
        return op->left;
    }

    bool Statement::Parse(Parser &parser, LocalScope &scope)
    {
        DEBUG("Parsing statement\n");
        this->parentScope = &scope;
        switch(parser.GetToken().type)
        {
            case TokenType::Number:
            case TokenType::Identifier:
            {
                this->type = StatementType::Expression;
                this->val = new Node();
                this->val->type = NodeType::Operand;
                Operand* op = new Operand();
                this->val->data.operand = op;
                op->Parse(parser, scope);
                if(parser.GetToken().type != TokenType::Semicolon)
                    parser.ErrorMsg("Expected end of statement");
            }
            break;


            default: 
            {
                parser.ErrorMsg("Invalid token whilst parsing statement");
            }
            break;
        }
        parser.NextToken();
        DEBUG("Finished parsing statement\n");
        return true;
    }

    bool IfStatement::Parse(Parser &parser, LocalScope &scope)
    {
        DEBUG("Parsing if statement\n");

        parser.ExpectNextToken(TokenType::OpenParam,
            "Expected definition of the if statement's condition");

        this->expression = new Operand();
        this->expression->Parse(parser, scope);
        this->hasExpression = true;

        if(parser.GetToken().type != TokenType::CloseParam)
            parser.ErrorMsg("Expected end of if statement declaration");

        this->scope = new LocalScope();
        this->scope->parent = new Node();
        this->scope->parent->type = NodeType::LocalScope;
        this->scope->parent->data.localScope = &scope;
        
        parser.ExpectNextToken(TokenType::OpenScope, 
            "Expected if statement scope");

        this->scope->Parse(parser);

        DEBUG("Finished parsing if statement\n");
        return true;
    }

    bool WhileLoop::Parse(Parser &parser)
    {

        return true;
    }

    bool Namespace::Parse(Parser &parser, Namespace *scope)
    {
        DEBUG("Parsing namespace\n")

        //Assign parent scope
        this->parent = scope;
        hasParent = scope == NULL ? false : true;

        if(hasParent)
        {
            //Get namespace identifier
            std::string id = parser.ExpectToken(TokenType::Identifier,
                "Expected namespace identifier\n").value;

            //Check for conflicting namespace identifiers in parent scope
            for(Namespace* name : scope->namespaces)
                if(name->identifier == id)
                    parser.ErrorMsg("Conflicting namespaces\n");

            this->identifier = id;

            //Check for proper declaration syntax
            parser.ExpectNextToken(
                TokenType::OpenScope,"Expected scope declaration\n");
        }
        else
        {
            identifier = "global";
        }

        parser.PushNewNamespace(this);

        //Parse namespace scope
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::Namespace:
                {
                    Namespace* newNamespace = new Namespace();
                    namespaces.push_back(newNamespace);
                    newNamespace->Parse(parser, this);
                }
                break;

                case TokenType::Struct:
                {
                    Structure* structure = new Structure();
                    types.push_back(structure);
                    structure->Parse(parser, *this);
                }
                break;

                case TokenType::Function:
                {
                    Function* func = new Function();
                    this->functions.push_back(func);
                    func->Parse(parser, *this);
                }
                break;

                // case TokenType::Identifier:
                // break;

                // case TokenType::Primitive:
                // break;

                default: 
                    parser.ErrorMsg("Unexpected token");
                    parser.NextToken();
                break;
            }
        }
        parser.NextToken();
        parser.PopNewNamespace();

        DEBUG("Finished parsing namespace %s\n", this->identifier.data());
        return true;
    }

    bool Namespace::ContainsFunction(std::string id)
    {
        for(Function* func : functions) 
            if(func->GetId() == id) return true;
        return false;
    }

    bool Namespace::ContainsStructure(std::string id)
    {
        for(Structure* type : types) 
            if(type->GetId() == id) return true;
        return false;
    }

    Variable* Namespace::GetVariable(std::string id)
    {
        return this->globals->GetVariable(id);
    }

    Function* Namespace::GetFunction(std::string id)
    {
        for(Function* var : this->functions)
        {
            if(var->GetId() == id) return var;
        }
        return NULL;
    }
}

namespace Silent
{
    using namespace Structures;
    Parser::Parser()
    {
        globalNamespace = new Structures::Namespace();
        tokenCursor = 0;
    }
    Parser::~Parser(){delete globalNamespace;}

    bool Parser::Parse(std::vector<Silent::Token> tokens)
    {
        ct = tokens[0];
        DEBUG("Started parsing\n");
        tokensPtr = &tokens;
        tokens.push_back({TokenType::CloseScope, "}", 0});
        if(!globalNamespace->Parse(*this, NULL))
            ERROR("Failed parsing global namespace\n");


        DEBUG("Parsing succsessfull\n");
        return true;
    }

    void Parser::ErrorMsg(std::string msg)
    {
        //ERROR((char*)"Parser error on line %llu:\n", ct.line);
        //ERROR(msg.data());
        //ERROR((char*)"At token: %s\n", ct.value.data());
        std::cout << "Parser error on line " << ct.line << "\n";
        std::cout << msg << "\n";
        std::cout << "At token: " << ct.value << "\n";
    }

    Token Parser::NextToken()
    {
        tokenCursor++;
        if(tokenCursor == tokensPtr->size()) tokenCursor--;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    Token Parser::PreviousToken()
    {
        tokenCursor--;
        if((long long)tokenCursor == -1) tokenCursor++;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    Token Parser::PeakToken(){return (*tokensPtr)[tokenCursor + 1];}
    Token Parser::GetToken(){return ct;}

    Token Parser::ExpectToken(TokenType type, std::string errMsg)
    {
        Token t = NextToken();
        if(t.type != type)
            ERROR("Error on line %llu:\n %s\n", t.line, errMsg.data())
        return t;
    }

    Token Parser::ExpectNextToken(TokenType type, std::string errMsg)
    {
        ExpectToken(type, errMsg);
        return NextToken();
    }

    DataType Parser::GetType(std::string id)
    {
        DataType t;
        t.type = new Node();
        t.type->type = NodeType::Primitive;
        if(id == "int8") 
        {
            t.type->data.primitiveType = Primitives::int8;
            t.size = 1;
        }
        else if(id == "uint8") 
        {
            t.type->data.primitiveType = Primitives::uint8;
            t.size = 1;
        }
        else if(id == "int16") 
        {
            t.type->data.primitiveType = Primitives::int16;
            t.size = 2;
        }
        else if(id == "uint16") 
        {
            t.type->data.primitiveType = Primitives::uint16;
            t.size = 2;
        }
        else if(id == "int32") 
        {
            t.type->data.primitiveType = Primitives::int32;
            t.size = 4;
        }
        else if(id == "uint32") 
        {
            t.type->data.primitiveType = Primitives::uint32;
            t.size = 4;
        }
        else if(id == "int64") 
        {
            t.type->data.primitiveType = Primitives::int64;
            t.size = 8;
        }
        else if(id == "uint64") 
        {
            t.type->data.primitiveType = Primitives::uint64;
            t.size = 8;
        }
        else if(id == "float32") 
        {
            t.type->data.primitiveType = Primitives::float32;
            t.size = 4;
        }
        else if(id == "float64") 
        {
            t.type->data.primitiveType = Primitives::float64;
            t.size = 8;
        }
        else if(id == "string") 
        {
            t.type->data.primitiveType = Primitives::string;
            t.size = 8;
        }
        else if(id == "pointer") 
        {
            t.type->data.primitiveType = Primitives::reference;
            t.size = 8;
        }
        else if(id == "void") 
        {
            t.type->data.primitiveType = Primitives::null;
            t.size = 0;
        }
        else
        {
            t.type->type = NodeType::Structure;
            bool found = false;
            for(int64 i = accessibleNamespaces.size()-1; i >= 0 && !found; i--)
            {
                Namespace* scope = accessibleNamespaces[i];
                for(Structure* structure : scope->GetStructs())
                {
                    if(structure->GetId() == id)
                    {
                        t.type->data.structure = structure;
                        t.size = structure->GetSize();
                        found = true;
                        break;
                    }
                }
            }
            if(found == false) t.type->type = NodeType::Undefined;
        }
        return t;
    }

    Variable* Parser::GetVariable(LocalScope &scope, std::string id)
    {
        Variable* var = scope.GetVariable(id);
        if(var == NULL)
        {
            switch(scope.GetParentType())
            {
                case NodeType::Namespace:
                    return GetVariable(*scope.parent->data.namespaceScope, id);
                break;

                case NodeType::LocalScope:
                    return GetVariable(*scope.parent->data.localScope, id);
                break;

                default: break;
            }
            return NULL;
        }
        else return var;
    }

    Variable* Parser::GetVariable(Namespace &scope, std::string id)
    {
        Variable* var = scope.GetVariable(id);
        if(var == NULL)
        {
            if(scope.parent != NULL) return GetVariable(*scope.parent, id);
            else return NULL;
        }
        else return var;
    }

    Function* Parser::GetFunction(std::string id)
    {
        for(uint64 i = accessibleNamespaces.size()-1; i >= 0; i--)
        {
            Namespace* scope = accessibleNamespaces[i];
            Function* func = scope->GetFunction(id); 
            if(func == NULL) continue;
            else return func;
        }
        return NULL;
    }

    void Parser::PushNewNamespace(Namespace* scope)
    {
        accessibleNamespaces.push_back(scope);
    }

    void Parser::PopNewNamespace()
    {
        accessibleNamespaces.pop_back();
    }

    Namespace* Parser::GetGlobalNamespace()
    {
        return globalNamespace;
    }
}