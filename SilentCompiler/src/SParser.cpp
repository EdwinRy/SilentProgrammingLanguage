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
    Parser::Parser()
    {
        this->tokensPtr = 0;
        this->tokenCursor = 0;
        this->errorCount = 0;
    }

    Node* Parser::Parse(std::vector<Silent::Token> tokens)
    {
        ct = tokens[0];
        tokensPtr = &tokens;

        tokens.push_back({TokenType::CloseScope, "}", 0});
        Node* program = new Node();

        while(GetToken().type != TokenType::CloseScope)
        {
            if(GetToken().type == TokenType::Namespace)
            {
                Node* n = ParseNamespaceDeclaration();
                if(n != NULL) program->children.push_back(n);
            }

            else
            {
                ErrorMsg("Invalid token in the global scope");
                this->NextToken();
            }
                
        }

        return NULL;
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

    Token Parser::Match(TokenType type)
    {
        if(type != this->ct.type)
        {
            this->ErrorMsg("Unexpected token");
        }
        return this->NextToken();
    }


    ////////////////////////////////////////////////////////////////////////////
    // PRODUCTION RULES

    //TOKENS
    Node* Parser::ParseIdentifier()
    {
        Node* n = NULL;
        if(GetToken().type == TokenType::Identifier)
        {
            Node* n = new Node();
            n->type = Node::Type::Identifier;
            n->value = GetToken().value;
            NextToken();
        }
        return n;
    }

    Node* Parser::ParseInteger()
    {

    }

    Node* Parser::ParseFloat()
    {

    }

    // <namespace-name> ::= <identifier> {"::" <identifier>}

    Node* Parser::ParseNamespaceName()
    {
        DEBUG("Namespace name\n");

        Node* n = ParseIdentifier();
        if(n == NULL) return NULL;

        while(GetToken().type == TokenType::ScopeResolution)
        {
            Match(TokenType::ScopeResolution);
            Node* n2 = ParseIdentifier();
            if(n2 == NULL) ErrorMsg("Unexpected token in namespace name");
            n->children.push_back(n2);
        }

        return n;

    }

    //SCOPES
    
    Node* Parser::ParseScope()
    {

    }

    //<namespace-scope> ::= "{" <namespace-statements> "}" 
    Node* Parser::ParseNamespaceScope()
    {
        Match(TokenType::OpenScope);
        Node* n = ParseNamespaceStatements();
        if(n == NULL)
        {
            ErrorMsg("Invalid Syntax");
            return NULL;
        }
        Match(TokenType::CloseScope);
        return n;
    }

    //<namespace-statements> ::= <namespace-statement>  {<namespace-statement>}
    Node* Parser::ParseNamespaceStatements()
    {
        DEBUG("Namespace statements\n");

        Node* n = ParseNamespaceStatement();
        if(n == NULL) 
        {
            ErrorMsg("Invalid syntax");
            return NULL;
        }

        for(;;)
        {
            Node* n2 = ParseNamespaceStatement();
            n->children.push_back(n2);
            if(n2 == NULL) break;
        }

        return n;
    }

    // <namespace-statement> ::= <function-declaration> | <type-declaration>
    Node* Parser::ParseNamespaceStatement()
    {
        DEBUG("Namespace statement\n");
    }



    //TYPES

    //DECLARATIONS
    // <namespace-declaration> ::= 
    //      "namespace" <namespace-name> <namespace-scope>
    Node* Parser::ParseNamespaceDeclaration()
    {
        DEBUG("Namespace declaration\n");
        this->Match(TokenType::Namespace);

        Node* nn = ParseNamespaceName();
        if(nn == NULL) 
        {
            ErrorMsg("Syntax Error");
            return NULL;
        }


        Node* ns = ParseNamespaceScope();
        if(ns == NULL) 
        {
            ErrorMsg("Syntax Error");
            return NULL;
        }

        Node* namespaceDeclaration = new Node();
        namespaceDeclaration->children.push_back(nn);
        namespaceDeclaration->children.push_back(ns);

        return namespaceDeclaration;
    }

}