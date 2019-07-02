#include "SParser2.hpp"

// Define debug output functions
#define DEBUG_ENABLED 1
#define ERROR(args...) printf(args);
#if DEBUG_ENABLED
#define DEBUG(args...) printf(args);
#else
#define DEBUG(args...) 
#endif

namespace Silent
{

    thread_local SymbolTable* SymbolTable::currentTable = 0;

    template <class Type>
    TableNode::TableNode(Type* node, NodeType type)
    {
        nodeType = type;
        this->node = (void*)node;
    }

    void SymbolTable::AddItem(TableNode node)
    {
        items.push_back(node);
    }

    void SymbolTable::AddChild(SymbolTable child)
    {
        children.push_back(child);
    }

    void SymbolTable::SetParent(SymbolTable* parent)
    {
        this->parent = parent;
    }

    SymbolTable* SymbolTable::GetParent()
    {
        return parent;
    }

    Parser::Parser()
    {
        this->tokensPtr = 0;
        this->tokenCursor = 0;
        this->errorCount = 0;
    }

    Program* Parser::Parse(std::vector<Silent::Token> tokens)
    {
        ct = tokens[0];
        tokensPtr = &tokens;

        tokens.push_back({TokenType::EndOfFile, "End of file", 0});
        Program* program = new Program();
        if(program->Parse(*this)) return program;
        else return NULL;
    }

    void Parser::ErrorMsg(std::string msg)
    {
        errorCount++;
        std::cout << "Parser error on line " << ct.line << "\n";
        std::cout << msg << "\n";
        std::cout << "At token: " << ct.value << "\n";
    }


    void Parser::WarningMsg(std::string msg)
    {
        std::cout << "Parser warning on line " << ct.line << "\n";
        std::cout << msg << "\n";
        std::cout << "At token: " << ct.value << "\n";
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

    Token Parser::NextToken(int offset)
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

    Token Parser::PeakToken(int offset)
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

    Token Parser::PreviousToken(int offset)
    {
        tokenCursor -= offset;
        if((long long)tokenCursor == -1) tokenCursor++;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    bool Parser::Match(TokenType type)
    {
        if(type == this->ct.type)
        {
            NextToken();
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

    bool Program::Parse(Parser &parser)
    {
        table.SetParent(NULL);
        SymbolTable::currentTable = &table;

        while(!parser.IsType(TokenType::EndOfFile))
        {
            if(parser.IsType(TokenType::Namespace))
            {
                NamespaceDeclaration nd;
                if(nd.Parse(parser)) namespaceDeclarations.push_back(nd);
            }

            else if(parser.IsType(TokenType::Type))
            {
                TypeDeclaration td;
                if(td.Parse(parser)) typeDeclarations.push_back(td);
            }

            else if(parser.IsType(TokenType::Function))
            {
                FunctionDeclaration fd;
                if(fd.Parse(parser)) functionDeclarations.push_back(fd);
            }

            else
            {
                parser.ErrorMsg("Invalid token in the global scope");
                parser.NextToken();
            }
            
        }
        return true;
    }

    // <namespace-scope> ::= 
    //     "{" { <function-declaration> | <type-declaration> } "}"
    bool NamespaceScope::Parse(Parser &parser)
    {
        // Get ptr to backtrack to
        uint64_t tokenPtr = parser.GetTokenCursor();

        // Check for open scope
        if(!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Failed to match namespace scope bracket");
            return false;
        }

        symTable.SetParent(SymbolTable::currentTable);
        SymbolTable::currentTable = &symTable;

        // Parse until met with scope-closing bracket
        while(!parser.Match(TokenType::CloseScope))
        {
            // Parse function declaration
            if(parser.Match(TokenType::Function))
            {
                FunctionDeclaration fd;
                if(fd.Parse(parser)) functionDeclarations.push_back(fd);
                else parser.ErrorMsg("Invalid syntax");
            }

            // Parse type declaration
            else if(parser.Match(TokenType::Type))
            {
                // TODO
            }

            else
            {
                parser.ErrorMsg("Invalid token in namespace scope");
                parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
                SymbolTable::currentTable = symTable.GetParent();
                SymbolTable::currentTable->AddChild(symTable);
                return false;
            }
            
        }

        SymbolTable::currentTable = symTable.GetParent();
        SymbolTable::currentTable->AddChild(symTable);
        return true;
    }

    // <namespace-declaration> ::= 
    //      "namespace" <identifier> <namespace-scope> 
    bool NamespaceDeclaration::Parse(Parser &parser)
    {
        // Get ptr to backtrack to
        uint64_t tokenPtr = parser.GetTokenCursor();

        // Check for syntax
        if(!parser.Match(TokenType::Namespace)) return false;

        // Get namespace identifier
        if(!parser.IsType(TokenType::Identifier))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }
        identifier = parser.GetTokenVal();
        parser.NextToken();

        SymbolTable::currentTable->AddItem(
            TableNode(this, TableNode::NodeType::Namespace));

        // Parse namespace scope
        if(!scope.Parse(parser))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            parser.ErrorMsg("Failed to parse namespace scope");
            return false;
        }

        return true;
    }

    bool Expression::Parse(Parser &parser)
    {
        return false;
    }

    // <variable-declaration> ::= <type-name> <identifier> ";"
    bool VariableDeclaration::Parse(Parser &parser)
    {
        // Get ptr to backtrack to
        uint64_t tokenPtr = parser.GetTokenCursor();

        if(!type.Parse(parser)) return false;
        if(!parser.IsType(TokenType::Identifier))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }

        id = parser.GetTokenVal();
        parser.NextToken();

        if(!parser.Match(TokenType::Semicolon))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }

        
        SymbolTable::currentTable->AddItem(
            TableNode(this,TableNode::NodeType::Variable));

        return true;
    }

    // <local-statement> ::= <variable-declaration> | <expression>
    bool LocalStatement::Parse(Parser &parser)
    {
        if(varDec.Parse(parser)) type = Type::VariableDeclaration;
        else if(expression.Parse(parser)) type = Type::Expression;
        else return false;
        return true;
    }

    // <local-scope> ::= "{" { <local-statement> } "}"
    bool LocalScope::Parse(Parser &parser)
    {

        TableNode func = SymbolTable::currentTable->GetItemBack();
        FunctionDeclaration* fDec;

        if(func.nodeType == TableNode::NodeType::Function) 
            fDec = (FunctionDeclaration*)func.GetNode();

        // Get ptr to backtrack to
        uint64_t tokenPtr = parser.GetTokenCursor();

        // Check for open scope
        if(!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Failed to match local scope bracket");
            return false;
        }

        symTable.SetParent(SymbolTable::currentTable);
        SymbolTable::currentTable = &symTable;

        if(func.nodeType == TableNode::NodeType::Function) 
        {
            Parameters params = fDec->GetParams();
            for(uint64_t i = 0; i < params.GetParamCount(); i++)
                symTable.AddItem(TableNode(params.GetParam(i), 
                    TableNode::NodeType::Variable));
        }

        // Parse until met with scope-closing bracket
        while(!parser.Match(TokenType::CloseScope))
        {
            LocalStatement statement;
            if(!statement.Parse(parser)) break;

            statements.push_back(statement);
        }

        SymbolTable::currentTable = symTable.GetParent();
        SymbolTable::currentTable->AddChild(symTable);

        return true;
    }

    // <type-name> ::= 
    //     <primitive> | <identifier> | <identifier> "::" <identifier>
    bool TypeName::Parse(Parser &parser)
    {
        if(parser.GetToken().IsPrimitive())
        {
            type = Type::Primitive;
            value = parser.GetTokenVal();
            parser.NextToken();
            return true;
        }
        else if(parser.IsType(TokenType::Identifier))
        {
            value = parser.GetTokenVal();
            parser.NextToken();
            if(parser.Match(TokenType::ScopeResolution))
            {
                if(parser.IsType(TokenType::Identifier))
                    value += "::" + parser.GetTokenVal();

                else
                {
                    parser.ErrorMsg("Invalid syntax for type name");
                    return false;
                }
            }
        }
        return false;
    }

    // <parameter> ::= <type-name> <identifier>
    bool Parameter::Parse(Parser &parser)
    {
        uint64_t tokenPtr = parser.GetTokenCursor();
        if(!type.Parse(parser)) return false;
        if(!parser.IsType(TokenType::Identifier))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }
        identifier = parser.GetTokenVal();
        parser.NextToken();
        return true;
    }

    // <parameters> ::= "(" [<parameter> {","<parameter>}] ")"
    bool Parameters::Parse(Parser &parser)
    {
        uint64_t tokenPtr = parser.GetTokenCursor();
        if(!parser.Match(TokenType::OpenParam)) return false;

        Parameter param;
        if(param.Parse(parser))
        {
            parameters.push_back(param);

            while(!parser.IsType(TokenType::CloseScope))
            {
                if(!parser.Match(TokenType::Comma)) break;
                if(!param.Parse(parser))
                {
                    parser.PreviousToken();
                    break;
                }
                parameters.push_back(param);
            }
        }

        if(!parser.Match(TokenType::CloseParam))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }

        return true;

    }

    // <function-declaration> ::= 
    //     "function" <identifier> <parameters> <function-scope> 
    bool FunctionDeclaration::Parse(Parser &parser)
    {
        // Get ptr to backtrack to
        uint64_t tokenPtr = parser.GetTokenCursor();

        // Check for syntax
        // if(!parser.Match(TokenType::Function)) return false;

        // Get namespace identifier
        if(!parser.IsType(TokenType::Identifier))
        {
            parser.PreviousToken();
            return false;
        }
        identifier = parser.GetTokenVal();
        parser.NextToken();

        // Parse parameters
        if(!parameters.Parse(parser))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }

        SymbolTable::currentTable->AddItem(
            TableNode(this, TableNode::NodeType::Function));

        if(!scope.Parse(parser))
        {
            parser.PreviousToken(parser.GetTokenCursor()-tokenPtr);
            return false;
        }

        // SymbolTable::AddItem(identifier, 
        //     TableNode(this, TableNode::NodeType::Function));
        return true;
    }

    bool TypeDeclaration::Parse(Parser &parser)
    {
        return false;
    }

    // <integer> ::= [<sign>] <unsigned_integer>
    bool Integer::Parse(Parser &parser)
    {
        if(parser.GetToken().IsSign())
        {
            value = parser.GetTokenVal();
            if(parser.NextToken().type == TokenType::Digits)
            {
                value = parser.GetTokenVal();
                parser.NextToken();
                return true;
            }
        }
        else
        {
            if(parser.GetTokenType() == TokenType::Digits)
            {
                value = parser.GetTokenVal();
                parser.NextToken();
                return true;
            }
        }
        return false;
    }

    bool Float::Parse(Parser &parser)
    {

    }

    // <literal> ::= <integer> | <float> | <string>
    bool Literal::Parse(Parser &parser)
    {

    }

    // <factor> ::= <literal> | "(" <expression> ")" | <function-call>
    Operand* Operand::ParseFactor(Parser &parser)
    {
        
    }

    Operand* Operand::ParseTerm(Parser &parser)
    {
        Operand* op = new Operand();
        Operand* temp = new Operand();
        op->left = ParseSum(parser);

        switch(parser.GetTokenType())
        {
            case TokenType::Multiply:
            case TokenType::Divide:
            {
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                parser.NextToken();
                op->right = ParseSum(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }

            default: break;
        }
        
        return op->left;
    }

    Operand* Operand::ParseSum(Parser &parser)
    {
        Operand* op = new Operand();
        Operand* temp = new Operand();
        op->left = ParseTerm(parser);

        switch(parser.GetTokenType())
        {
            case TokenType::Add:
            case TokenType::Subtract:
            {
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                parser.NextToken();
                op->right = ParseTerm(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }

            default: break;
        }
        
        return op->left;
    }

    Operand* Operand::ParseLogic(Parser &parser)
    {
        Operand* op = new Operand();
        Operand* temp = new Operand();
        op->left = ParseSum(parser);

        switch(parser.GetTokenType())
        {
            case TokenType::And:
            case TokenType::Or:
            case TokenType::Xor:
            {
                op->type = Operand::TokenToOperandType(parser.GetTokenType());
                parser.NextToken();
                op->right = ParseSum(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }

            default: break;
        }
        
        return op->left;
    }

    Operand* Operand::ParseComparison(Parser &parser)
    {

    }

    Operand* Operand::ParseConditional(Parser &parser)
    {

    }

    Operand::Type Operand::TokenToOperandType(TokenType tokenType)
    {
        switch(tokenType)
        {
            case TokenType::And: return Type::And; break;
            case TokenType::Or: return Type::Or; break;
            case TokenType::Xor: return Type::Xor; break;
            case TokenType::Add: return Type::Add; break;
            case TokenType::Subtract: return Type::Sub; break;
            case TokenType::Multiply: return Type::Mul; break;
            case TokenType::Divide: return Type::Div; break;
        }
    }
}