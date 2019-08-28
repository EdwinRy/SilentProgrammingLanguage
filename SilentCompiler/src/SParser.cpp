#include "SParser.hpp"
#include "SHelper.hpp"
#include <string>

namespace Silent
{
namespace Parser
{
    thread_local SymbolTable* SymbolTable::currentTable = 0;

    template <class T>
    TableNode::TableNode(T* node, Type type)
    {
        nodeType = type;
        this->node = (void*)node;
    }

    TableNode::TableNode()
    {
        this->nodeType = Type::Null;
        this->node = NULL;
    }

    SymbolTable::SymbolTable()
    {
        parent = NULL;
    }

    void SymbolTable::AddItem(TableNode node)
    {
         items.push_back(node);
    }

    void SymbolTable::AddChild(SymbolTable *child)
    {
        children.push_back(child);
    }

    void SymbolTable::SetParent(SymbolTable *parent)
    {
        this->parent = parent;
    }

    SymbolTable* SymbolTable::GetParent()
    {
        return parent;
    }

    Parser::Parser()
    {
        this->warningCount = 0;
        this->tokensPtr = 0;
        this->tokenCursor = 0;
        this->errorCount = 0;
        this->errorsEnabled = true;
        this->warningsEnabled = true;
        this->debuggingEnabled = true;
    }

    Program* Parser::Parse(std::vector<Silent::Token> tokens)
    {
        ct = tokens[0];
        tokensPtr = &tokens;

        tokens.push_back({TokenType::EndOfFile, "End of file", 0});
        Program* program = new Program();
        if (program->Parse(*this)) 
        {
            DebugMsg("\033[1;34mParsing Finished with:");
            DebugMsg("\033[1;31m" + std::to_string(errorCount) + " Errors\033[0m and");
            DebugMsg("\033[1;33m" + std::to_string(warningCount) + " Warnings\033[0m");
            return program;
        }
        else return NULL;
    }

    void Parser::ErrorMsg(std::string msg)
    {
        errorCount++;
        if (errorsEnabled)
        {
            std::cout 
                << "\033[1;31m" << "Parser error on " << "\033[1;4;31m"
                << "line " << ct.line << ":\033[0m\n"
                << "\033[4;31m" << msg << "\033[0m\n"
                << "\033[1;31mAt token: " << ct.value << "\033[0m\n";
        }
        if (errorCount > 5000) (void)NextToken(2);
    }


    void Parser::WarningMsg(std::string msg)
    {
        warningCount++;
        if (warningsEnabled)
        {
            std::cout 
                << "\033[1;33m" << "Parser warning on " << "\033[1;4;33m" 
                << "line " << ct.line << ":\033[0m\n"
                << "\033[4;33m" << msg << "\033[0m\n"
                << "\033[1;33m" << "Near token: " << ct.value << "\033[0m\n";
        }
    }

    void Parser::DebugMsg(std::string msg)
    {
        if (debuggingEnabled)
        {
            std::cout << msg << "\n";
        }
    }

    Token Parser::GetToken()
    {
        return ct;
    }

    uint64_t Parser::GetTokenCursor()
    {
        return tokenCursor;
    }

    Token Parser::NextToken()
    {
        tokenCursor++;
        if(tokenCursor == tokensPtr->size()) tokenCursor--;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    Token Parser::NextToken(uint64_t offset)
    {
        tokenCursor += offset;
        if(tokenCursor == tokensPtr->size()) tokenCursor--;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    Token Parser::PeakToken()
    {
        return (*tokensPtr)[tokenCursor + 1];
    }

    Token Parser::PeakToken(uint64_t offset)
    {
        return (*tokensPtr)[tokenCursor + offset];
    }

    Token Parser::PreviousToken()
    {
        tokenCursor--;
        if((long long)tokenCursor == -1) tokenCursor++;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    Token Parser::PreviousToken(uint64_t offset)
    {
        tokenCursor -= offset;
        if((long long)tokenCursor == -1) tokenCursor++;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    void Parser::Revert(uint64_t revertPtr)
    {
        (void)PreviousToken(GetTokenCursor()-revertPtr);
    }

    bool Parser::Match(TokenType type)
    {
        if(type == this->ct.type)
        {
            (void)NextToken();
            return true;
        }
        return false;
    }

    bool Parser::IsType(TokenType type)
    {
        if(type == ct.type) return true;
        else return false;
    }

    std::string Parser::GetTokenVal()
    {
        return ct.value;
    }

    TokenType Parser::GetTokenType()
    {
        return ct.type;
    }


    // Parse program
    bool Program::Parse(Parser &parser)
    {
        // TODO: Add self functionality to the rest of the parser
        
        parser.DebugMsg("\033[36mParsing program\033[0m");
        table = new SymbolTable();
        table->self = TableNode(this, TableNode::Type::Program);
        SymbolTable::currentTable = table;

        /*types.push_back(new Type("Void", 0));
        types.push_back(new Type("int8", 1));
        types.push_back(new Type("uint8", 1));
        types.push_back(new Type("int16", 2));
        types.push_back(new Type("uint16", 2));
        types.push_back(new Type("int32", 4));
        types.push_back(new Type("uint32", 4));
        types.push_back(new Type("int64", 8));
        types.push_back(new Type("uint64", 8));
        types.push_back(new Type("float32", 4));
        types.push_back(new Type("float64", 8));
        types.push_back(new Type("string", 8));*/

        while(!parser.IsType(TokenType::EndOfFile))
        {
            switch(parser.GetTokenType())
            {
                // Parse namespace definition
                case TokenType::Namespace:
                {
                    Namespace* newNamespace = new Namespace();
                    if(newNamespace->Parse(parser))
                    {
                        namespaces.push_back(newNamespace);
                    }
                    else
                    {
                        delete newNamespace;
                        parser.WarningMsg("Invalid namespace definition");
                        (void)parser.NextToken();
                    }
                    
                }
                break;

                // Parse function definition
                case TokenType::Subroutine:
                {
                    Subroutine* function = new Subroutine();
                    if(function->Parse(parser))
                    {
                        functions.push_back(function);
                    }
                    else
                    {
                        delete function;
                        parser.WarningMsg("Invalid function definition");
                        (void)parser.NextToken();
                    }
                }
                break;

                // Parse type definition
                case TokenType::Type:
                {
                    Type* type = new Type();
                    if (type->Parse(parser))
                    {
                        types.push_back(type);
                    }
                    else
                    {
                        delete type;
                        parser.WarningMsg("Invalid type definition");
                        (void)parser.NextToken();
                    }
                }
                break;

                default:
                {
                    parser.ErrorMsg("Invalid token in program scope");
                    (void)parser.NextToken(2);
                }
                break;
            }
        }
        parser.DebugMsg("\033[32mFinished parsing program\033[0m");
        parser.DebugMsg("Resulting parse tree:");
        //SymTablePrinter::PrintSymTable(this->table);
        return true;
    }

    bool Namespace::Parse(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing namespace\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();

        if(!ParseDeclaration(parser))
        {
            parser.ErrorMsg("Failed to parse namespace declaration");
            parser.Revert(revertPtr);
            return false;
        }

        // Parse namespace scope
        if (!ParseScope(parser))
        {
            // TODO: delete symbol table
            SymbolTable::currentTable = symTable->GetParent();
            SymbolTable::currentTable->AddChild(symTable);
            parser.ErrorMsg("Failed to parse namespace scope");
            parser.Revert(revertPtr);
            return false;
        }

        parser.DebugMsg("\033[32mFinished parsing namespace\033[0m");
        return true;
    }

    bool Namespace::ParseDeclaration(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing namespace declaration\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();
        // Check syntax
        parser.Match(TokenType::Namespace);

        // Get namespace identifier
        if(!ParseIdentifier(parser))
        {
            parser.ErrorMsg("Failed to parse namespace identifier");
            parser.Revert(revertPtr);
            return false;
        }

        parser.DebugMsg(
            "\033[32mFinished parsing namespace declaration\033[0m");
        return true;
    }

    bool Namespace::ParseIdentifier(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing namespace identifier\033[0m");
        if(parser.GetTokenType() == TokenType::Identifier)
        {
            id = parser.GetTokenVal();
            (void)parser.NextToken();
            parser.DebugMsg(
                "\033[32mFinished parsing namespace identifier\033[0m");
            return true;
            
        }
        else
        {
            parser.WarningMsg(
                "\033[32mFailed parsing namespace identifier\033[0m");
            return false;
        }
    }

    bool Namespace::ParseScope(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing namespace scope\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();

        // Check for open scope
        if(!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Failed to match namespace scope bracket");
            parser.Revert(revertPtr);
            return false;
        }

        symTable = new SymbolTable();
        symTable->self = TableNode(this, TableNode::Type::Namespace);
        symTable->SetParent(SymbolTable::currentTable);
        SymbolTable::currentTable->AddChild(symTable);
        SymbolTable::currentTable->AddItem(symTable->self);
        SymbolTable::currentTable = symTable;

        while(!parser.Match(TokenType::CloseScope))
        {
            switch (parser.GetTokenType())
            {
                // Parse function declaration
                case TokenType::Subroutine:
                {
                    Subroutine* function = new Subroutine();
                    if(function->Parse(parser))
                    {
                        functions.push_back(function);
                    }
                    else 
                    {
                        delete function;
                        parser.ErrorMsg("Invalid function declaration syntax");
                        parser.Revert(revertPtr);
                        SymbolTable::currentTable = symTable->GetParent();
                        return false;
                    }
                }
                break;

                // Parse type declaration
                case TokenType::Type:
                {

                }
            
                default:
                {
                    parser.ErrorMsg("Invalid token in namespace scope");
                    parser.Revert(revertPtr);
                    SymbolTable::currentTable = symTable->GetParent();
                    return false;
                }
                break;
            }
        }

        SymbolTable::currentTable = symTable->GetParent();
        parser.DebugMsg("\033[32mFinished parsing namespace scope\033[0m");
        return true;
    }

    Variable::Variable(Parameter* param)
    {
        this->id = param->id;
        this->type = param->type;
        this->scopeIndex = 0;
    }

    bool Variable::Parse(Parser &parser)
    {
        uint64_t revertPtr = parser.GetTokenCursor();
        if (type.Parse(parser))
        {
            if (parser.IsType(TokenType::Identifier))
            {
                parser.DebugMsg("Parsing variable declaration");
                id = parser.GetTokenVal();
                (void)parser.NextToken();
                parser.DebugMsg("Got variable identifier: " + id);

                if (parser.Match(TokenType::Semicolon))
                {
                    parser.DebugMsg("Finished parsing variable declaration");
                    SymbolTable::currentTable->AddItem(
                        TableNode(this, TableNode::Type::Variable));
                    return true;
                }
                else if (parser.Match(TokenType::Assign))
                {
                    if (init.Parse(parser))
                    {
                        parser.DebugMsg(
                            "Finished parsing variable declaration");
                        SymbolTable::currentTable->AddItem(
                            TableNode(this, TableNode::Type::Variable));
                        return true;
                    }
                    else
                    {
                        parser.ErrorMsg("Failed parsing variable init");
                        parser.Revert(revertPtr);
                        return false;
                    }
                }
                else
                {
                    parser.ErrorMsg(
                        "Invalid token following variable declaration");
                    parser.Revert(revertPtr);
                }
            
            }
            else
            {
                parser.DebugMsg("Failed variable declaration production");
                parser.Revert(revertPtr);
                return false;
            }
        }
        else return false;
    }

    bool Literal::Parse(Parser& parser)
    {
        uint64_t revertPtr = parser.GetTokenCursor();
        if (parser.GetTokenType() == TokenType::StringValue)
        {
            type = Type::String;
            val = parser.GetTokenVal();
            (void)parser.NextToken();
            return true;
        }

        if(parser.IsType(TokenType::Add) || parser.IsType(TokenType::Subtract))
        {
            val = parser.GetTokenVal();
            (void)parser.NextToken();
        }

        if (parser.IsType(TokenType::Digits))
        {
            val += parser.GetTokenVal();
            (void)parser.NextToken();
            if (parser.Match(TokenType::Fullstop))
            {
                if (parser.IsType(TokenType::Digits))
                {
                    type = Type::Float;
                    val += "." + parser.GetTokenVal();
                    return true;
                }
                else
                {
                    parser.ErrorMsg("Failed float production rule");
                    parser.Revert(revertPtr);
                    return false;
                }
            }
            else
            {
                type = Type::Integer;
                return true;
            }
        }
        else
        {
            parser.Revert(revertPtr);
            return false;
        }
    }

    // <factor> ::= <literal> | "(" <expression> ")" | <function-call>
    //              | <expression-lhs>
    Operand* Operand::ParseFactor(Parser& parser)
    {
        parser.DebugMsg("Parsing factor");
        Operand* op = new Operand();
        op->type = Operand::Type::Factor;
        Literal lit = {};
        if (lit.Parse(parser))
        {
            //op->node = TableNode(new Literal(lit), TableNode::Type::Literal);
            return op;
        }

        ExpressionLHS expName = {};
        if (expName.Parse(parser))
        {
            //op->node = TableNode(
            //    new ExpressionLHS(expName), 
            //    TableNode::Type::ExpressionLHS);
            return op;
        }
        // TODO: Add other productions
        delete op;
        return NULL;
    }

    Operand* Operand::ParseTerm(Parser& parser)
    {
        Operand *op = new Operand();
        Operand *temp;

        op->left = ParseFactor(parser);

        switch (parser.GetTokenType())
        {
            case TokenType::Multiply:
            case TokenType::Divide:
            {
                parser.DebugMsg("Parsing term");
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                (void)parser.NextToken();
                op->right = ParseFactor(parser);
                return op;
            }
            break;

            default: 
            {
                temp = op->left;
                delete op;
                return temp;
            }
            break;
        }
    }

    Operand* Operand::ParseSum(Parser& parser)
    {
        Operand* op = new Operand();
        Operand* temp;

        op->left = ParseTerm(parser);

        switch (parser.GetTokenType())
        {
            case TokenType::Add:
            case TokenType::Subtract:
            {
                parser.DebugMsg("Parsing sum");
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                (void)parser.NextToken();
                op->right = ParseTerm(parser);
                return op;
            }
            break;

            default:
            {
                temp = op->left;
                delete op;
                return temp;
            }
            break;
        }
    }

    Operand* Operand::ParseLogic(Parser& parser)
    {
        Operand* op = new Operand();
        Operand* temp;

        op->left = ParseSum(parser);

        switch (parser.GetTokenType())
        {
            case TokenType::And:
            case TokenType::Or:
            case TokenType::Not:
            {
                parser.DebugMsg("Parsing logic");
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                (void)parser.NextToken();
                op->right = ParseSum(parser);
                return op;
            }
            break;

            default:
            {
                temp = op->left;
                delete op;
                return temp;
            }
            break;
        }
    }

    Operand* Operand::ParseComparison(Parser& parser)
    {
        Operand* op = new Operand();
        Operand* temp;

        op->left = ParseLogic(parser);

        switch (parser.GetTokenType())
        {
            case TokenType::Equal:
            case TokenType::NotEqual:
            case TokenType::LessThan:
            case TokenType::LessThanOrEqualTo:
            case TokenType::MoreThan:
            case TokenType::MoreThanOrEqualTo:
            {
                parser.DebugMsg("Parsing comparison");
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                (void)parser.NextToken();
                op->right = ParseLogic(parser);
                return op;
            }
            break;

            default:
            {
                temp = op->left;
                delete op;
                return temp;
            }
            break;
        }
    }

    Operand* Operand::ParseConditional(Parser& parser)
    {
        Operand* op = new Operand();
        Operand* temp;

        op->left = ParseComparison(parser);

        switch (parser.GetTokenType())
        {
            case TokenType::ConditionalAnd:
            case TokenType::ConditionalOr:
            {
                parser.DebugMsg("Parsing conditional");
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                (void)parser.NextToken();
                op->right = ParseComparison(parser);
                return op;
            }
            break;

            default:
            {
                temp = op->left;
                delete op;
                return temp;
            }
            break;
        }
    }

    Operand* Operand::ParseAssignment(Parser& parser)
    {
        Operand* op = new Operand();
        Operand* temp;

        op->left = ParseConditional(parser);

        switch (parser.GetTokenType())
        {
            case TokenType::Assign:
            case TokenType::AddAssign:
            case TokenType::SubtractAssign:
            case TokenType::MultiplyAssign:
            case TokenType::DivideAssign:
            {
                parser.DebugMsg("Parsing assignment");
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                (void)parser.NextToken();
                op->right = ParseConditional(parser);
                return op;
            }
            break;

            default:
            {
                temp = op->left;
                delete op;
                return temp;
            }
            break;
        }
    }

    Operand::Type Operand::TokenToOperandType(TokenType tokenType)
    {
        switch (tokenType)
        {
            case TokenType::And: return Type::And; break;
            case TokenType::Or: return Type::Or; break;
            case TokenType::Xor: return Type::Xor; break;
            case TokenType::Add: return Type::Add; break;
            case TokenType::Subtract: return Type::Sub; break;
            case TokenType::Multiply: return Type::Mul; break;
            case TokenType::Divide: return Type::Div; break;

            case TokenType::Assign: return Type::Assign; break;
            case TokenType::AddAssign: return Type::AddAssign; break;
            case TokenType::SubtractAssign: return Type::SubtractAssign; break;
            case TokenType::MultiplyAssign: return Type::MultiplyAssign; break;
            case TokenType::DivideAssign: return Type::DivideAssign; break;

            default: return Type::None; break;
        }
    }

    bool Expression::Parse(Parser &parser)
    {
        parser.DebugMsg("Parsing expression");
        op = Operand::ParseAssignment(parser);
        if (!parser.Match(TokenType::Semicolon))
        {
            parser.ErrorMsg("Semicolon expected at the end of expression");
        }
        return op != NULL;
    }

    bool ExpressionLHS::ObjectAccess::Parse(Parser& parser)
    {
        uint64_t revertPtr = parser.GetTokenCursor();
        while (parser.Match(TokenType::Fullstop))
        {
            if (parser.IsType(TokenType::Identifier))
            {
                access.push_back(parser.GetTokenVal());
            }
            else
            {
                parser.ErrorMsg("Invalid expression name syntax");
                parser.Revert(revertPtr);
                return false;
            }
        }

        if (access.size() == 0) return false;
        else return true;
    }

    bool ExpressionLHS::ArrayAccess::Parse(Parser& parser)
    {
        uint64_t revertPtr = parser.GetTokenCursor();
        while (parser.Match(TokenType::OpenBracket))
        {
            Expression ex;
            if (ex.Parse(parser))
            {
                if (!parser.Match(TokenType::CloseBracket))
                {
                    parser.WarningMsg("Expected closing bracket");
                    return false;
                }
                access.push_back(ex);
            }
            else
            {
                parser.ErrorMsg("Expected expression in array access");
                parser.Revert(revertPtr);
                return false;
            }
        }

        if (access.size() == 0) return false;
        else return true;
    }

    bool ExpressionLHS::Parse(Parser& parser)
    {
        uint64_t revertPtr = parser.GetTokenCursor();
        if (parser.IsType(TokenType::Identifier))
        {
            id = parser.GetTokenVal();
            (void)parser.NextToken();

            ObjectAccess oa;
            if (oa.Parse(parser)) objectAccess = new ObjectAccess(oa);

            ArrayAccess aa;
            if (aa.Parse(parser)) arrayAccess = new ArrayAccess(aa);

            return true;
        }
        else return false;
    }


    bool Subroutine::Parse(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing subroutine\033[0m");

        uint64_t revertPtr = parser.GetTokenCursor();

        // Check for syntax
        if(!parser.Match(TokenType::Subroutine))
        {
            parser.ErrorMsg("Invalid subroutine definition");
            parser.Revert(revertPtr);
            return false;
        }

        // Get function return type
        if (!ParseReturnType(parser))
        {
            parser.ErrorMsg("Could not parse subroutine return type");
            parser.Revert(revertPtr);
            return false;
        }

        // Get function identifier
        if(!ParseIdentifier(parser))
        {
            parser.ErrorMsg("Could not parse subroutine identifier");
            parser.Revert(revertPtr);
            return false;
        }

        // Get function parameters
        if(!ParseParameters(parser))
        {
            parser.ErrorMsg("Could not parse subroutine parameters");
            parser.Revert(revertPtr);
            return false;
        }

        // Parse function scope
        if(!ParseScope(parser))
        {
            parser.ErrorMsg("Could not parse subroutine scope");
            parser.Revert(revertPtr);
            return false;
        }

        parser.DebugMsg("\033[32mFinished parsing subroutine\033[0m");
        return true;
    }

    bool Subroutine::ParseIdentifier(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing subroutine identifier\033[0m");
        if(parser.GetTokenType() == TokenType::Identifier)
        {
            id = parser.GetTokenVal();
            (void)parser.NextToken();
            parser.DebugMsg("Got subroutine identifier: " + id);
            parser.DebugMsg(
                "\033[32mFinished parsing subroutine identifier\033[0m");
            return true;
        }
        else
        {
            parser.DebugMsg("Parsing subroutine identifier has failed");
            return false;
        }
    }

    bool Subroutine::ParseReturnType(Parser& parser)
    {
        parser.DebugMsg("\033[36mParsing subroutine return type\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();

        if (!returnType.Parse(parser))
        {
            parser.WarningMsg("Parsing subroutine return type has failed");
            parser.Revert(revertPtr);
            return false;
        }

        parser.DebugMsg("Got subroutine return type: " + returnType.value);
        return true;
    }

    bool Subroutine::ParseParameters(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing subroutine parameters\033[0m");

        uint64_t revertPtr = parser.GetTokenCursor();

        if(!parser.Match(TokenType::OpenParam))
        {
            parser.ErrorMsg("Expected open bracket for parameters");
            parser.Revert(revertPtr);
            return false;
        }

        Parameter param = {};
        if(param.Parse(parser))
        {
            params.push_back(new Parameter(param));

            while(!parser.IsType(TokenType::CloseParam))
            {
                param = {};
                if (!parser.Match(TokenType::Comma))
                {
                    parser.WarningMsg("Expected parameter separator");
                    break;
                }
                if(!param.Parse(parser))
                {
                    parser.WarningMsg("Failed to parse subroutine parameter");
                    (void)parser.PreviousToken();
                    break;
                }
                params.push_back(new Parameter(param));
            }
        }

        if(!parser.Match(TokenType::CloseParam))
        {
            parser.ErrorMsg("Expected end of parameters");
            parser.Revert(revertPtr);
            return false;
        }

        parser.DebugMsg(
            "\033[32mFinished parsing subroutine parameters\033[0m");

        return true;
    }

    bool Parameter::Parse(Parser &parser)
    {
        parser.DebugMsg("Parsing subroutine parameter");
        uint64_t revertPtr = parser.GetTokenCursor();

        if(!type.Parse(parser)) return false;
        if(!parser.IsType(TokenType::Identifier))
        {
            parser.Revert(revertPtr);
            return false;
        }
        id = parser.GetTokenVal();
        (void)parser.NextToken();

        parser.DebugMsg("Finished parsing subroutine parameter: " + id);
        return true;
    }

    bool Subroutine::ParseScope(Parser &parser)
    {
        parser.DebugMsg("\033[36mParsing subroutine scope\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();

        symTable = new SymbolTable();
        symTable->self = TableNode(this, TableNode::Type::Subroutine);
        symTable->SetParent(SymbolTable::currentTable);
        SymbolTable::currentTable->AddChild(symTable);
        SymbolTable::currentTable->AddItem(symTable->self);
        SymbolTable::currentTable = symTable;

        // Syntax check
        if(!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Expected open scope");
            parser.Revert(revertPtr);
            SymbolTable::currentTable = symTable->GetParent();
            return false;
        }

        // Parse scope body
        while(!parser.Match(TokenType::CloseScope))
        {
            LocalStatement statement;
            if(!statement.Parse(parser)) break;
            statements.push_back(statement);
        }


        parser.DebugMsg("\033[32mFinished parsing subroutine scope\033[0m");
        SymbolTable::currentTable = symTable->GetParent();
        return true;
    }

    bool LocalStatement::Parse(Parser &parser)
    {

        Variable *newVarDec = new Variable();
        if (newVarDec->Parse(parser))
        {
            varDec = newVarDec;
            type = Type::VariableDeclaration;
            return true;
        }
        delete newVarDec;

        Expression *newExpression = new Expression();
        if (newExpression->Parse(parser))
        {
            expression = newExpression;
            type = Type::Expression;
            return true;
        }
        delete newExpression;

        parser.ErrorMsg("Invalid local statement");
        return false;
    }

    bool Type::Parse(Parser& parser)
    {
        parser.DebugMsg("\033[36mParsing type\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();

        if (!parser.Match(TokenType::Type))
        {
            parser.ErrorMsg("Invalid type definition");
            parser.Revert(revertPtr);
            return false;
        }

        if (!ParseIdentifier(parser))
        {
            parser.ErrorMsg("Failed to parse type identifier");
            parser.Revert(revertPtr);
            return false;
        }

        if (!ParseScope(parser))
        {
            parser.ErrorMsg("Failed to parse type scope");
            parser.Revert(revertPtr);
            return false;
        }

        parser.DebugMsg("\033[32mFinished parsing type\033[0m");
        return true;
    }

    bool Type::ParseIdentifier(Parser& parser)
    {
        parser.DebugMsg("\033[36mParsing type identifier\033[0m");
        if (parser.GetTokenType() == TokenType::Identifier)
        {
            id = parser.GetTokenVal();
            (void)parser.NextToken();
            parser.DebugMsg("Got type identifier: " + id);
            parser.DebugMsg(
                "\033[32mFinished parsing type identifier\033[0m");
            return true;
        }
        else
        {
            parser.DebugMsg("Parsing type identifier has failed");
            return false;
        }
    }


    Type::Member::AccessModifier Type::Member::currentAccessModifier;

    bool Type::ParseScope(Parser& parser)
    {
        parser.DebugMsg("\033[36mParsing type scope\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();

        symTable = new SymbolTable();
        symTable->self = TableNode(this, TableNode::Type::Structure);
        symTable->SetParent(SymbolTable::currentTable);
        SymbolTable::currentTable->AddChild(symTable);
        SymbolTable::currentTable->AddItem(symTable->self);
        SymbolTable::currentTable = symTable;

        // Syntax check
        if (!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Expected open scope");
            parser.Revert(revertPtr);
            SymbolTable::currentTable = symTable->GetParent();
            return false;
        }

        // Parse scope body
        while (!parser.Match(TokenType::CloseScope))
        {
            if (parser.GetToken().IsAccessModifier() &&
                parser.PeakToken().type == TokenType::Semicolon)
            {
                Member::currentAccessModifier = 
                    Member::TokenToAccess(parser.GetTokenType());
                (void)parser.NextToken(2);
            }

            Member member;
            if (member.Parse(parser, this))
            {
                members.push_back(new Member(member));
            }
            else
            {
                parser.ErrorMsg("Failed to parse type member");
                parser.Revert(revertPtr);
                SymbolTable::currentTable = symTable->GetParent();
                return false;
            }
        }

        parser.DebugMsg("\033[32mFinished parsing type scope\033[0m");
        SymbolTable::currentTable = symTable->GetParent();
        return true;
    }

    bool Type::Member::Parse(Parser& parser, Type* parent)
    {
        parser.DebugMsg("\033[36mParsing type member\033[0m");
        uint64_t revertPtr = parser.GetTokenCursor();
        parentType = parent;
        // Get access modifier
        if (parser.GetToken().IsAccessModifier())
        {
            accessModifier = TokenToAccess(parser.GetTokenType());
            (void)parser.NextToken();
        }
        
        // Parse method
        if (parser.GetTokenType() == TokenType::Subroutine)
        {
            Subroutine* sub = new Subroutine();
            if (sub->Parse(parser))
            {
                memberType = MemberType::Method;
                method = sub;
                return true;
            }
            else
            {
                delete sub;
                parser.ErrorMsg("Failed to parse type method");
                parser.Revert(revertPtr);
                return false;
            }
        }

        // Parse attribute
        else
        {
            Attribute *attrib = new Attribute();
            if (attrib->Parse(parser))
            {
                memberType = MemberType::Attribute;
                attribute = attrib;
                return true;
            }
            else
            {
                delete attrib;
                parser.ErrorMsg("Failed to parse type attribute");
                parser.Revert(revertPtr);
                return false;
            }
        }

        parser.DebugMsg("\033[32mFinished parsing type member\033[0m");
    }

    Type::Member::AccessModifier Type::Member::TokenToAccess(TokenType type)
    {
        switch (type)
        {
            case TokenType::Public:
                return AccessModifier::Public;
            break;
            case TokenType::Private:
                return AccessModifier::Protected;
            break;
            case TokenType::Protected:
                return AccessModifier::Protected;
            break;
            
            default:
                return AccessModifier::Public;
            break;
        }
    }

    bool Attribute::Parse(Parser& parser)
    {
        uint64_t revertPtr = parser.GetTokenCursor();
        if (type.Parse(parser))
        {
            if (parser.IsType(TokenType::Identifier))
            {
                parser.DebugMsg("Parsing attribute declaration");
                id = parser.GetTokenVal();
                (void)parser.NextToken();
                parser.DebugMsg("Got attribute identifier: " + id);

                if (parser.Match(TokenType::Semicolon))
                {
                    parser.DebugMsg("Finished parsing attribute declaration");
                    SymbolTable::currentTable->AddItem(
                        TableNode(this, TableNode::Type::Variable));
                    return true;
                }
                else
                {
                    parser.ErrorMsg(
                        "Invalid token following attribute declaration");
                    parser.Revert(revertPtr);
                    return false;
                }
            }

            else
            {
                parser.DebugMsg("Failed attribute declaration production");
                parser.Revert(revertPtr);
                return false;
            }
        }
        else return false;
    }

    bool TypeName::Parse(Parser &parser)
    {
        if(parser.GetToken().IsPrimitive())
        {
            type = Type::Primitive;
            value = parser.GetTokenVal();
            (void)parser.NextToken();
            return true;
        }
        else if(parser.IsType(TokenType::Identifier))
        {
            value = parser.GetTokenVal();
            (void)parser.NextToken();
            if(parser.Match(TokenType::ScopeResolution))
            {
                if(parser.IsType(TokenType::Identifier))
                {
                    value += "::" + parser.GetTokenVal();
                    (void)parser.NextToken();
                    return true;
                }

                else
                {
                    (void)parser.PreviousToken(2);
                    parser.ErrorMsg("Invalid syntax for type name");
                    return false;
                }
            }
            else
            {
                (void)parser.PreviousToken();
                return false;
            }
        }
        return true;
    }
}}