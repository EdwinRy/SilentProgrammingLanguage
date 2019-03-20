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

    void Parser::WarningMsg(std::string msg)
    {
        errorCount++;
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

        ScopeResolution scopeReference;

        // Generate the namespace address from global scope
        // if(namespaceIdentifier.substr(0,2) == "::")
        if(namespaceIdentifier.compare(0, 2, "::") == 0)
        {
            ScopeResolution tempScopeReference(namespaceIdentifier);
            scopeReference = tempScopeReference;
        }
        else
        {
            ScopeResolution tempScopeReference(
                parent.scopeFormatted + "::" + namespaceIdentifier);
            scopeReference = tempScopeReference;
        }

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
            tableNode->node.nodeType = Node::Type::Namespace;
            
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
            tableNode->node.module = thisNamespace;
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
        else parser.ErrorMsg("Unexpected token in namespace declaration");
        
        // Parse Namespace scope
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            switch(parser.GetToken().type)
            {
                case TokenType::Namespace:
                {
                    Namespace* newNamespace = new Namespace();
                    newNamespace->Parse(parser, scopeReference);
                }
                break;

                case TokenType::Function:
                {
                    Function* newFunction = new Function();
                    newFunction->Parse(parser, scopeReference);
                }
                break;

                default:parser.NextToken(); parser.NextToken();break;
            }
        }
        parser.NextToken();
        DEBUG("Finished parsing namespace\n");
    }

    void Function::Parse(Parser &parser, ScopeResolution parent)
    {
        DEBUG("Parsing Function\n");

        // Skip through the function keyword
        parser.NextToken();

        // Check for function definition syntax
        if(parser.GetToken().type != TokenType::Identifier)
            parser.ErrorMsg("Unexpected token in function declaration");

        // Get the return type of the function
        ScopeResolution returnTypeReference = 
            ScopeResolution(parser.GetToken().value);
        parser.NextToken();

        // Get function identifier
        std::string functionIdentifier = parser.GetToken().value;
        parser.NextToken();

        ScopeResolution functionReference;
        // Generate table reference
        if(functionIdentifier.compare(0, 2, "::") == 0)
        {
            ScopeResolution tempFunctionReference(functionIdentifier);
            functionReference = tempFunctionReference;
        }
        else
        {
            ScopeResolution tempFunctionReference(
                parent.scopeFormatted + "::" + functionIdentifier);
            functionReference = tempFunctionReference;
        }

        DEBUG("Got function reference %s\n", 
            functionReference.scopeFormatted.data());

        // Prepare space for a node in the table
        SymTableNode* tableNode;

        // Pointer for the function to be parsed
        Function* thisFunction;

        // If the function is not in the symbol table
        if(SymTableNode::symTable[functionReference] == NULL)
        {
            // Create new symbol table node
            DEBUG("Function definition\n");
            tableNode = new SymTableNode();
            tableNode->scopeReference = functionReference;
            tableNode->node.nodeType = Node::Type::Function;
            
            // Check if the parent of the function is not global
            if(parent.scopeFormatted != "")
                // Append the node to the parent's children
                SymTableNode::symTable[parent]->
                    children.push_back(tableNode);

            // Add the node to the symbol table
            SymTableNode::symTable[functionReference] = tableNode;

            // Create the namespace object
            thisFunction = new Function();
            thisFunction->identifier = functionIdentifier;
            thisFunction->scopeResolution = 
                new ScopeResolution(functionReference);
            thisFunction->returnType = new ScopeResolution(returnTypeReference);
            tableNode->node.function = thisFunction;
        }
        // If the function is already declared
        else
        {
            parser.WarningMsg("Function " + functionIdentifier + " at " + 
                functionReference.scopeFormatted + 
                "has already been declared and so it " 
                "will be overriden by the following declaration");

            tableNode = SymTableNode::symTable[functionReference];

            // TODO: add node type check
            thisFunction = tableNode->node.function;
            thisFunction->returnType = new ScopeResolution(returnTypeReference);
        }

        // Check for either open scope indicating initialisation of the scope
        // or semicolon for just the declaration
        if(parser.GetToken().type == TokenType::OpenScope)
            parser.NextToken();
        else if(parser.GetToken().type == TokenType::Semicolon)
        {
            parser.NextToken();
            return;
        }
        else parser.ErrorMsg("Unexpected token in function declaration");

        // Parse function scope
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            Statement statement;
            thisFunction->statements.push_back(Statement());
            switch(parser.GetToken().type)
            {
                case TokenType::Asm:
                {

                }
                break;

                case TokenType::Identifier:
                {

                }
                break;

                case TokenType::If:
                {

                }
                break;

                case TokenType::IfNot:
                {

                }
                break;

                default:parser.NextToken(); parser.NextToken();break;
            }
        }
        parser.NextToken();
        DEBUG("Finished parsing function\n");
    }
}