#include "SParser.hpp"
#include <sstream>


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
    thread_local 
        std::unordered_map<ScopeResolution, SymTableNode*> SymTableNode::symTable;

    ScopeResolution::ScopeResolution(std::string scope)
    {
        this->scopeFormatted = scope;
        // Split scope string into a list of scope strings
        std::string scopeBuffer;
        for(unsigned long long i = 0; i < scope.length(); i++)
        {
            if(scope[i] == (char)':' && scope[i+1] == (char)':')
            {
                i++;
                scopeList.push_back(scopeBuffer);
                scopeBuffer = "";
            }
            else
            {
                scopeBuffer += scope[i];
            }
        }
        scopeList.push_back(scopeBuffer);
    }

    ScopeResolution::ScopeResolution(std::vector<std::string> scope)
    {
        this->scopeList = scope;
        // Join scope list into a string
        std::string lastScope = scope.back();
        scope.pop_back();
        for(std::string scopeName : scope)
        {
            scopeFormatted += scopeName;
            scopeFormatted += "::";
        }
        scopeFormatted += lastScope;
    }

    ScopeResolution& ScopeResolution::operator+=(std::string rhs)
    {
        // Split added string and add it to scope array
        std::string scopeBuffer;
        for(unsigned long long i = 0; i < rhs.length(); i++)
        {
            if(rhs[i] == (char)':' && rhs[i+1] == (char)':')
            {
                i++;
                scopeList.push_back(scopeBuffer);
                scopeBuffer = "";
            }
            else
            {
                scopeBuffer += rhs[i];
            }
        }
        scopeList.push_back(scopeBuffer);

        // Regenerate scope reference stirng
        this->scopeFormatted = "";
        std::string lastScope = scopeList.back();
        scopeList.pop_back();
        for(std::string scopeName : scopeList)
        {
            scopeFormatted += scopeName;
            scopeFormatted += "::";
        }
        scopeFormatted += lastScope;
        scopeList.push_back(lastScope);
        return *this;
    }

    bool ScopeResolution::operator==(const ScopeResolution& rhs) const
    {
        return this->scopeFormatted == rhs.scopeFormatted;
    }

    ScopeResolution& ScopeResolution::operator--()
    {
        this->scopeList.pop_back();
        this->scopeFormatted = "";
        std::string lastScope = scopeList.back();
        scopeList.pop_back();
        for(std::string scopeName : scopeList)
        {
            scopeFormatted += scopeName;
            scopeFormatted += "::";
        }
        scopeFormatted += lastScope;
        scopeList.push_back(lastScope);
        return *this;
    }

    Parser::Parser()
    {
        tokenCursor = 0;
        errorCount = 0;
    }

    void Parser::ErrorMsg(std::string msg)
    {
        errorCount++;
        std::cout << "Parser error on line " << ct.line << "\n";
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

    Token Parser::PeakToken()
    {
        return (*tokensPtr)[tokenCursor + 1];
    }

    Token Parser::PreviousToken()
    {
        tokenCursor--;
        if((long long)tokenCursor == -1) tokenCursor++;
        ct = (*tokensPtr)[tokenCursor];
        return ct;
    }

    SymTableNode Parser::Parse(std::vector<Silent::Token> tokens)
    {
        DEBUG("Started parsing\n");
        ct = tokens[0];

        tokensPtr = &tokens;
        tokens.push_back({TokenType::CloseScope, "}", 0});

        while(this->GetToken().type != TokenType::CloseScope)
        {
            switch(this->GetToken().type)
            {
                case TokenType::Namespace:
                    Types::Namespace::Parse(*this, ScopeResolution(""));
                break;

                case TokenType::Function:
                {

                }
                break;

                default:
                break;
            }


        }
        this->NextToken();

        printf("Parsing successful with %llu errors\n", errorCount);
        return symTable;
    }
};

namespace Silent::Types
{
    void Namespace::Parse(Parser &parser, ScopeResolution parent)
    {
        DEBUG("Parsing Namespace\n");

        // Skip through namespace keyword
        parser.NextToken(); 

        // Check for namespace definition syntax
        if(parser.GetToken().type != TokenType::Identifier)
            parser.ErrorMsg("Unexpected token in namespace declaration");

        // Get the namespace identifier
        std::string namespaceIdentifier = parser.GetToken().value;
        parser.NextToken();

        // Generate the namespace address from global scope
        ScopeResolution scopeReference(
            parent.scopeFormatted + "::" + namespaceIdentifier);

        DEBUG("Got scope resolution %s\n",scopeReference.scopeFormatted.data());

        // Prepare space for a node in the table
        SymTableNode* tableNode;

        // Pointer for the namespace to be parsed
        Namespace* thisNamespace;

        // If the namespace is not in the symbol table
        if(SymTableNode::symTable[scopeReference] == NULL)
        {
            // Create new symbol table node
            DEBUG("New namespace definition\n");
            tableNode = new SymTableNode();
            tableNode->scopeReference = scopeReference;
            
            // Check if the parent of the namespace is not global
            if(parent.scopeFormatted != "")
                // Append the node to the parent's children
                SymTableNode::symTable[parent]->
                    children.push_back(tableNode);

            // Add the node to the symbol table
            SymTableNode::symTable[scopeReference] = tableNode;

            // Create the namespace object
            thisNamespace = new Namespace();
            thisNamespace->identifier = namespaceIdentifier;
            thisNamespace->scopeResolution =new ScopeResolution(scopeReference);
        }
        // If the namespace is already declared
        else
        {
            DEBUG("Appending to already existing namespace\n");
            tableNode = SymTableNode::symTable[scopeReference];

            // TODO: add node type check
            thisNamespace = tableNode->node.module;

        }
        


        // Check for either open scope indicating initialisation of members
        // or semicolon for just the declaration
        if(parser.GetToken().type == TokenType::OpenScope)
            parser.NextToken();
        else if(parser.GetToken().type == TokenType::Semicolon)
        {
            parser.NextToken();
            return;
        }
        else
        {
            parser.ErrorMsg("Unexpected token in namespace declaration");
        }


        
        // Parse Namespace scope
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::Namespace:
                {
                    Namespace* newNamespace = new Namespace();
                    // namespaces.push_back(newNamespace);
                    newNamespace->Parse(parser, scopeReference);
                }
                break;

                default:break;
            }
        }
        parser.NextToken();
        DEBUG("Finished parsing namespace\n");
        
    }
}