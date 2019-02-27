#include "SParser.hpp"
#include <sstream>
#define DEBUG_ENABLED 0
#define ERROR(args...) printf(args);
#if DEBUG_ENABLED
#define DEBUG(args...) printf(args);
#else
#define DEBUG(args...) 
#endif

namespace Silent
{
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
        ct = tokens[0];

        DEBUG("Started parsing\n");
        tokensPtr = &tokens;
        tokens.push_back({TokenType::CloseScope, "}", 0});

        while(this->GetToken().type != TokenType::CloseScope)
        {
            switch(this->GetToken().type)
            {
                case TokenType::Namespace:
                    Types::Namespace::Parse(*this);
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
        // Skip through namespace keyword
        parser.NextToken();

        if(parser.GetToken().type != TokenType::Identifier)
        {
            parser.ErrorMsg("Unexpected token in namespace declaration");
        }

        SymTableNode* tableNode = new SymTableNode();
        ScopeResolution scopeReference(parent += parser.GetToken().value);

        if(parser.GetSymData().count(scopeReference) == 1)
        {

        }
        
    }
}