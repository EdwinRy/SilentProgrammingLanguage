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
        std::unordered_map<ScopeResolution, SymTableNode*> 
            SymTableNode::symTable;

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
                    Types::Function::Parse(*this, ScopeResolution(""));
                break;

                default:
                    this->ErrorMsg("Invalid token in the global scope");
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
                    Namespace::Parse(parser, scopeReference);
                break;

                case TokenType::Function:
                    Function::Parse(parser, scopeReference);
                break;

                default: parser.NextToken(); parser.NextToken(); break;
            }
        }
        parser.NextToken();
        DEBUG("Finished parsing namespace\n");
    }

    void Function::ParseParameters(Parser &parser)
    {
        // Skip parameter's open bracket
        parser.NextToken();

        while(parser.GetToken().type != TokenType::CloseParam)
        {
            // Create new variable
            Variable param;
            
            // Get variable type;
            param.type = new ScopeResolution(parser.GetToken().value);

            // Get variable identifier
            param.identifier = parser.NextToken().value;

            if(parser.NextToken().type == TokenType::Comma)
                parser.NextToken();

            DEBUG("Adding parameter %s of type %s\n", 
                param.identifier.data(),
                param.type->scopeFormatted.data()
            );

            this->parameters.push_back(param);
        }

        // Skip parameter's closing bracket
        parser.NextToken();
    }

    void Function::Parse(Parser &parser, ScopeResolution parent)
    {
        DEBUG("Parsing Function\n");

        // Skip through the function keyword
        parser.NextToken();

        // Check for function definition syntax
        if(parser.GetToken().type != TokenType::Identifier &&
            !parser.GetToken().IsPrimitive())
            parser.ErrorMsg("Unexpected token in function declaration");

        // Get the return type of the function
        ScopeResolution returnTypeReference = 
            ScopeResolution(parser.GetToken().value);
        parser.NextToken();

        DEBUG("Got function return type: %s\n", 
            returnTypeReference.scopeFormatted.data());

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

        // Parse function parameters
        thisFunction->ParseParameters(parser);

        // Check for either open scope indicating initialisation of the scope
        // or semicolon for just the declaration
        if(parser.GetToken().type == TokenType::OpenScope)
        {
            parser.NextToken();
        }
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
            thisFunction->statements.push_back(statement);
            statement.Parse(parser, functionReference);
        }
        parser.NextToken();
        DEBUG("Finished parsing function\n");
    }

    void Statement::Parse(Parser &parser, ScopeResolution parent)
    {
        switch(parser.GetToken().type)
        {
            case TokenType::Asm:
            {
                this->StatementType = Type::Asm;
                this->assembly = new Asm();
                this->assembly->Parse(parser);
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

            case TokenType::OpenParam:
            case TokenType::NumberValue:
            {
                Operand::ParseExpression(parser);
            }
            break;

            default:parser.NextToken(); parser.NextToken();break;
        }
    }

    AsmInstruction::OpcodeType AsmInstruction::StrToOpcode(std::string str)
    {
        if(str == "Halt") return OpcodeType::Halt;
        else if(str == "Goto") return OpcodeType::Goto;

        else if(str == "Call") return OpcodeType::Call;
        else if(str == "Return") return OpcodeType::Return;
        else if(str == "LoadDll") return OpcodeType::LoadDll;
        else if(str == "LoadDllFunc") return OpcodeType::LoadDllFunc;
        else if(str == "FreeDll") return OpcodeType::FreeDll;
        else if(str == "CallDllFunc") return OpcodeType::CallDllFunc;

        else if(str == "Push1") return OpcodeType::Push1;
        else if(str == "Push2") return OpcodeType::Push2;
        else if(str == "Push4") return OpcodeType::Push4;
        else if(str == "Push8") return OpcodeType::Push8;
        else if(str == "PushX") return OpcodeType::PushX;

        else if(str == "Pop1") return OpcodeType::Pop1;
        else if(str == "Pop2") return OpcodeType::Pop2;
        else if(str == "Pop4") return OpcodeType::Pop4;
        else if(str == "Pop8") return OpcodeType::Pop8;
        else if(str == "PopX") return OpcodeType::PopX;

        else if(str == "Store1") return OpcodeType::Store1;
        else if(str == "Store2") return OpcodeType::Store2;
        else if(str == "Store4") return OpcodeType::Store4;
        else if(str == "Store8") return OpcodeType::Store8;
        else if(str == "StoreX") return OpcodeType::StoreX;

        else if(str == "Load1") return OpcodeType::Load1;
        else if(str == "Load2") return OpcodeType::Load2;
        else if(str == "Load4") return OpcodeType::Load4;
        else if(str == "Load8") return OpcodeType::Load8;
        else if(str == "LoadX") return OpcodeType::LoadX;

        else if(str == "StoreGlobal1") return OpcodeType::StoreGlobal1;
        else if(str == "StoreGlobal2") return OpcodeType::StoreGlobal2;
        else if(str == "StoreGlobal4") return OpcodeType::StoreGlobal4;
        else if(str == "StoreGlobal8") return OpcodeType::StoreGlobal8;
        else if(str == "StoreGlobalX") return OpcodeType::StoreGlobalX;

        else if(str == "LoadGlobal1") return OpcodeType::LoadGlobal1;
        else if(str == "LoadGlobal2") return OpcodeType::LoadGlobal2;
        else if(str == "LoadGlobal4") return OpcodeType::LoadGlobal4;
        else if(str == "LoadGlobal8") return OpcodeType::LoadGlobal8;
        else if(str == "LoadGlobalX") return OpcodeType::LoadGlobalX;

        else if(str == "Alloc1") return OpcodeType::Alloc1;
        else if(str == "Alloc2") return OpcodeType::Alloc2;
        else if(str == "Alloc4") return OpcodeType::Alloc4;
        else if(str == "Alloc8") return OpcodeType::Alloc8;
        else if(str == "AllocX") return OpcodeType::AllocX;

        else if(str == "LoadPtr1") return OpcodeType::LoadPtr1;
        else if(str == "LoadPtr2") return OpcodeType::LoadPtr2;
        else if(str == "LoadPtr4") return OpcodeType::LoadPtr4;
        else if(str == "LoadPtr8") return OpcodeType::LoadPtr8;
        else if(str == "LoadPtrX") return OpcodeType::LoadPtrX;

        else if(str == "StorePtr1") return OpcodeType::StorePtr1;
        else if(str == "StorePtr2") return OpcodeType::StorePtr2;
        else if(str == "StorePtr4") return OpcodeType::StorePtr4;
        else if(str == "StorePtr8") return OpcodeType::StorePtr8;
        else if(str == "StorePtrX") return OpcodeType::StorePtrX;
        else if(str == "Free") return OpcodeType::Free;
        else if(str == "GetPtr") return OpcodeType::GetPtr;

        else if(str == "AddI1") return OpcodeType::AddI1;
        else if(str == "AddI2") return OpcodeType::AddI2;
        else if(str == "AddI4") return OpcodeType::AddI4;
        else if(str == "AddI8") return OpcodeType::AddI8;
        else if(str == "AddF4") return OpcodeType::AddF4;
        else if(str == "AddF8") return OpcodeType::AddF8;

        else if(str == "SubI1") return OpcodeType::SubI1;
        else if(str == "SubI2") return OpcodeType::SubI2;
        else if(str == "SubI4") return OpcodeType::SubI4;
        else if(str == "SubI8") return OpcodeType::SubI8;
        else if(str == "SubF4") return OpcodeType::SubF4;
        else if(str == "SubF8") return OpcodeType::SubF8;

        else if(str == "MulI1") return OpcodeType::MulI1;
        else if(str == "MulI2") return OpcodeType::MulI2;
        else if(str == "MulI4") return OpcodeType::MulI4;
        else if(str == "MulI8") return OpcodeType::MulI8;
        else if(str == "MulF4") return OpcodeType::MulF4;
        else if(str == "MulF8") return OpcodeType::MulF8;

        else if(str == "DivI1") return OpcodeType::DivI1;
        else if(str == "DivI2") return OpcodeType::DivI2;
        else if(str == "DivI4") return OpcodeType::DivI4;
        else if(str == "DivI8") return OpcodeType::DivI8;
        else if(str == "DivF4") return OpcodeType::DivF4;
        else if(str == "DivF8") return OpcodeType::DivF8;

        else if(str == "SmallerThan") return OpcodeType::SmallerThan;
        else if(str == "SmallerThanOrEqual") 
            return OpcodeType::SmallerThanOrEqual;
        else if(str == "LargerThan") return OpcodeType::LargerThan;
        else if(str == "LargerThanOrEqual") 
            return OpcodeType::LargerThanOrEqual;
        else if(str == "Equal") return OpcodeType::Equal;
        else if(str == "NotEqual") return OpcodeType::NotEqual;

        else if(str == "If") return OpcodeType::If;
        else if(str == "IfNot") return OpcodeType::IfNot;
        else if(str == "And") return OpcodeType::And;
        else if(str == "Or") return OpcodeType::Or;
        else if(str == "Xor") return OpcodeType::Xor;
        else if(str == "Not") return OpcodeType::Not;
        else return OpcodeType::Halt;
    }

    void AsmInstruction::Parse(Parser &parser)
    {
        // Get instruction opcode
        this->Opcode = StrToOpcode(parser.GetToken().value);

        DEBUG("Got opcode %s\n", parser.GetToken().value.data());

        // Parse operands
        char i = 0;
        while(parser.NextToken().type != TokenType::Semicolon && i < 4)
        {
            values.push_back(parser.GetToken().value);
            DEBUG("Got operand %s\n", parser.GetToken().value.data());
            i++;
        }
        parser.NextToken();
    }


    void Asm::Parse(Parser &parser)
    {
        DEBUG("Parsing asm scope\n");
        // Skip asm keyword
        parser.NextToken();

        if(parser.GetToken().type != TokenType::OpenScope)
            parser.ErrorMsg("Expected assembly scope opening");
        parser.NextToken();
        
        while(parser.GetToken().type != TokenType::CloseScope)
        {
            DEBUG("Parsing asm instruction\n");
            AsmInstruction asmInstruction;
            asmInstruction.Parse(parser);
            instructions.push_back(asmInstruction);
        }
    }

    Operand* Operand::ParseFactor(Parser &parser)
    {
        switch(parser.GetToken().type)
        {
            case TokenType::NumberValue:
            {
                Operand *op = new Operand();
                op->value = new Node();
                op->Operator = OperatorType::None;
                op->value->nodeType = Node::Type::Value;
                op->value->strValue = new std::string(parser.GetToken().value);
                DEBUG("Got factor %s\n", op->value->strValue->data());
                parser.NextToken();
                return op;
            }
            break;

            case TokenType::OpenParam:
            {
                parser.NextToken();
                Operand* op = Operand::ParseExpression(parser);

                if(parser.GetToken().type != TokenType::CloseParam)
                    parser.ErrorMsg("expected closing brackets");
                parser.NextToken();
                return op;
            }
            break;

            default: parser.ErrorMsg("Invalid operand in expression"); break;
        }
        return NULL;
    }
    
    Operand* Operand::ParseTerm(Parser &parser)
    {
        DEBUG("Parsing term\n");
        Operand* op = new Operand();
        Operand* temp = new Operand();
        op->left = ParseFactor(parser);

        switch(parser.GetToken().type)
        {
            case TokenType::Multiply:
            {
                op->Operator = OperatorType::Multiply;
                parser.NextToken();
                op->right = ParseFactor(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;

            case TokenType::Divide:
            {
                op->Operator = OperatorType::Divide;
                parser.NextToken();
                op->right = ParseFactor(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;
        }
        return op->left;
    }
    
    Operand* Operand::ParseSum(Parser &parser)
    {
        DEBUG("Parsing sum\n");
        Operand* op = new Operand();
        Operand* temp = new Operand();
        op->left = ParseTerm(parser);

        switch(parser.GetToken().type)
        {
            case TokenType::Add:
            {
                op->Operator = OperatorType::Add;
                parser.NextToken();
                op->right = ParseTerm(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;

            case TokenType::Subtract:
            {
                op->Operator = OperatorType::Substract;
                parser.NextToken();
                op->right = ParseTerm(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;
        }
        return op->left;
    }
    
    Operand* Operand::ParseLogic(Parser &parser)
    {
        DEBUG("Parsing logical operation\n");
        Operand* op = new Operand();
        Operand* temp = new Operand();
        op->left = ParseSum(parser);

        switch(parser.GetToken().type)
        {
            case TokenType::And:
            {
                op->Operator = OperatorType::Add;
                parser.NextToken();
                op->right = ParseSum(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;

            case TokenType::Or:
            {
                op->Operator = OperatorType::Or;
                parser.NextToken();
                op->right = ParseSum(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;

            case TokenType::Xor:
            {
                op->Operator = OperatorType::Xor;
                parser.NextToken();
                op->right = ParseSum(parser);
                temp = op;
                op = new Operand();
                op->left = temp;
            }
            break;
        }
        return op->left;
    }
    
    Operand* Operand::ParseComparison(Parser &parser)
    {
        // TODO: implement the rest of this sub
        return Operand::ParseLogic(parser);
    }

    Operand* Operand::ParseExpression(Parser &parser)
    {
        DEBUG("Parsing expression\n");

        Operand *op = ParseComparison(parser);

        return op;
    }
}