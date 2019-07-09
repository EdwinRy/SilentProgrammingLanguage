#include "SParser.hpp"

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

    void SymbolTable::SetParent(SymbolTable *parent)
    {
        this->parent = parent;
    }

    void SymbolTable::SetupChild()
    {
        SetParent(currentTable);
        currentTable = this;
    }

    void SymbolTable::AddCurrentChild()
    {
        currentTable = this->GetParent();
        currentTable->AddChild(*this);
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

    void Parser::Revert(uint64_t revertPtr)
    {
        PreviousToken(GetTokenCursor()-revertPtr);
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

    // Parse program
    bool Program::Parse(Parser &parser)
    {
        DEBUG("Parsing program\n");

        table.SetParent(NULL);
        SymbolTable::currentTable = &table;

        while(!parser.IsType(TokenType::EndOfFile))
        {
            switch(parser.GetTokenType())
            {
                // Parse namespace definition
                case TokenType::Namespace:
                {
                    Namespace newNamespace;
                    if(newNamespace.Parse(parser))
                    {
                        namespaces.push_back(newNamespace);
                    }
                    else
                    {
                        parser.WarningMsg("Invalid namespace definition");
                        parser.NextToken();
                    }
                    
                }
                break;

                // Parse function definition
                case TokenType::Function:
                {
                    Function function;
                    if(function.Parse(parser))
                    {
                        functions.push_back(function);
                    }
                    else
                    {
                        parser.WarningMsg("Invalid function definition");
                        parser.NextToken();
                    }
                }
                break;

                // Parse type definition
                case TokenType::Type:
                {

                }
                break;

                default:
                {
                    parser.ErrorMsg("Invalid token in program scope");
                    parser.NextToken(2);
                }
                break;
            }
        }
        
        DEBUG("Finished parsing program\n");
    }

    bool Namespace::Parse(Parser &parser)
    {
        DEBUG("Parsing namespace\n");
        uint64_t revertPtr = parser.GetTokenCursor();

        if(!ParseDeclaration(parser))
        {
            parser.ErrorMsg("Failed to parse namespace declaration");
            parser.Revert(revertPtr);
            return false;
        }

        DEBUG("Finished parsing namespace\n");
        return true;
    }

    bool Namespace::ParseDeclaration(Parser &parser)
    {
        DEBUG("Parsing namespace declaration\n");
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

        SymbolTable::currentTable->
            AddItem(TableNode(this,TableNode::NodeType::Namespace));
        symTable.SetupChild();

        // Parse namespace scope
        if(!ParseScope(parser))
        {
            symTable.AddCurrentChild();
            parser.ErrorMsg("Failed to parse namespace scope");
            parser.Revert(revertPtr);
            return false;
        }

        symTable.AddCurrentChild();
        DEBUG("Finished parsing namespace declaration\n");
        return true;
    }

    bool Namespace::ParseIdentifier(Parser &parser)
    {
        if(parser.GetTokenType() == TokenType::Identifier)
        {
            id = parser.GetTokenVal();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Namespace::ParseScope(Parser &parser)
    {
        DEBUG("Parsing namespace scope\n");
        uint64_t revertPtr = parser.GetTokenCursor();

        // Check for open scope
        if(!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Failed to match namespace scope bracket");
            parser.Revert(revertPtr);
            return false;
        }

        while(!parser.Match(TokenType::CloseScope))
        {
            switch (parser.GetTokenType())
            {
                // Parse function declaration
                case TokenType::Function:
                {
                    Function function;
                    if(function.Parse(parser))
                    {
                        functions.push_back(function);
                    }
                    else 
                    {
                        parser.ErrorMsg("Invalid function declaration syntax");
                        parser.Revert(revertPtr);
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
                    return false;
                }
                break;
            }
        }

        DEBUG("Finished parsing namespace scope\n");
        return true;
    }

    bool Function::Parse(Parser &parser)
    {
        DEBUG("Parsing function\n");

        uint64_t revertPtr = parser.GetTokenCursor();

        // Check for syntax
        if(!parser.Match(TokenType::Function))
        {
            parser.ErrorMsg("Invalid function definition");
            parser.Revert(revertPtr);
            return false;
        }

        // Get function identifier
        if(!ParseIdentifier(parser))
        {
            parser.ErrorMsg("Could not parse function identifier");
            parser.Revert(revertPtr);
            return false;
        }

        // Get function parameters
        if(!ParseParameters(parser))
        {
            parser.ErrorMsg("Could not parse function parameters");
            parser.Revert(revertPtr);
            return false;
        }

        // Parse funcion scope
        if(!ParseScope(parser))
        {
            parser.ErrorMsg("Could not parse function scope");
            parser.Revert(revertPtr);
            return false;
        }


        DEBUG("Finished parsing function\n");
        return true;
    }

    bool Function::ParseIdentifier(Parser &parser)
    {
        if(parser.GetTokenType() == TokenType::Identifier)
        {
            id = parser.GetTokenVal();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Function::ParseParameters(Parser &parser)
    {
        DEBUG("Parsing function parameters\n");

        uint64_t revertPtr = parser.GetTokenCursor();

        if(!parser.Match(TokenType::OpenParam))
        {
            parser.ErrorMsg("Expected open bracket for parameters");
            parser.Revert(revertPtr);
            return false;
        }

        Parameter param;
        if(param.Parse(parser))
        {
            params.push_back(param);

            while(!parser.IsType(TokenType::CloseParam))
            {
                if(!parser.Match(TokenType::Comma)) break;
                if(!param.Parse(parser))
                {
                    parser.PreviousToken();
                    break;
                }
                params.push_back(param);
            }
        }

        if(!parser.Match(TokenType::CloseParam))
        {
            parser.ErrorMsg("Expected end of parameters");
            parser.Revert(revertPtr);
            return false;
        }

        DEBUG("Finished parsing function parameters\n");
        return true;
    }

    bool Function::Parameter::Parse(Parser &parser)
    {
        DEBUG("Parsing parameter\n");
        uint64_t revertPtr = parser.GetTokenCursor();

        if(!type.Parse(parser)) return false;
        if(!parser.IsType(TokenType::Identifier))
        {
            parser.Revert(revertPtr);
            return false;
        }
        id = parser.GetTokenVal();
        parser.NextToken();

        DEBUG("Finished parsing parameter\n");
        return true;
    }

    bool Function::ParseScope(Parser &parser)
    {
        DEBUG("Parsing function scope\n");
        uint64_t revertPtr = parser.GetTokenCursor();

        // Syntax check
        if(!parser.Match(TokenType::OpenScope))
        {
            parser.ErrorMsg("Expected open scope");
            parser.Revert(revertPtr);
            return false;
        }

        // Parse scope body
        while(!parser.Match(TokenType::CloseScope))
        {
            LocalStatement statement;
            if(!statement.Parse(parser)) break;
            statements.push_back(statement);
        }


        DEBUG("Finished parsing function scope\n");
        return true;
    }

    bool Function::LocalStatement::Parse(Parser &parser)
    {
        if(varDec.Parse(parser)) type = Type::VariableDeclaration;
        else if(expression.Parse(parser)) type = Type::Expression;
        else 
        {
            parser.ErrorMsg("Invalid local statement");
            return false;
        }
        return true;
    }


    bool Type::Name::Parse(Parser &parser)
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
                {
                    value += "::" + parser.GetTokenVal();
                    parser.NextToken();
                    return true;
                }

                else
                {
                    parser.PreviousToken(2);
                    parser.ErrorMsg("Invalid syntax for type name");
                    return false;
                }
            }
            else
            {
                parser.PreviousToken();
                return false;
            }
        }
    }


}