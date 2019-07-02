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

    Node::Node(Node::Type nodeType) { type = nodeType; }

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

        tokens.push_back({TokenType::EndOfFile, "End of file", 0});
        Node* program = new Node();

        while(GetToken().type != TokenType::EndOfFile)
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

    bool Parser::Match(TokenType type)
    {
        if(type == this->ct.type)
        {
            NextToken();
            return true;
        }
        return false;
    }

    void Parser::DeleteNode(Node* node)
    {
        if(node == NULL) return;
        for(Node* child : node->children)
            if(child != NULL) DeleteNode(child);
        delete node;
    }


    ////////////////////////////////////////////////////////////////////////////
    // PRODUCTION RULES

    // TOKENS

    Node* Parser::ParseDigits()
    {
        if(GetToken().type == TokenType::Digits)
        {
            DEBUG("Digits\n");
            Node* digits = new Node(Node::Type::Digits);
            digits->value = GetToken().value;
            NextToken();
        }
        return NULL;
    }

    Node* Parser::ParseString()
    {
        if(GetToken().type == TokenType::String)
        {
            DEBUG("String\n");
            Node* digits = new Node(Node::Type::String);
            digits->value = GetToken().value;
            NextToken();
        }
        return NULL;
    }

    Node* Parser::ParseIdentifier()
    {
        if(GetToken().type == TokenType::Identifier)
        {
            DEBUG("Identifier\n");
            Node* identifier = new Node(Node::Type::Identifier);
            identifier->value = GetToken().value;
            NextToken();
            return identifier;
        }
        return NULL;
    }

    //<integer> ::= [<sign>] <digits>
    Node* Parser::ParseInteger()
    {
        Node* sign = ParseSign();

        if(sign == NULL)
        {
            Node* digits = ParseDigits();
            if(digits == NULL) return NULL;

            Node* integer = new Node(Node::Type::Integer);
            integer->children.push_back(digits);
            DEBUG("Integer\n");
            return integer;
        }

        else
        {
            Node* digits = ParseDigits();
            if(digits == NULL) 
            {
                DeleteNode(sign);
                PreviousToken();
                return NULL;
            }

            Node* integer = new Node(Node::Type::Integer);
            integer->children.push_back(sign);
            integer->children.push_back(digits);
            DEBUG("Integer\n");
            return integer;
        }
    }

    // <sign> ::= "+" | "-"
    Node* Parser::ParseSign()
    {
        DEBUG("Sign\n");

        if(GetToken().IsSign())
        {
            Node* n = new Node(Node::Type::Sign);
            n->value = GetToken().value;
            NextToken();
            return n;
            DEBUG("Sign\n");
        }
        return NULL;
    }

    // <float> ::= [<sign>] <digits> "." <digits>
    Node* Parser::ParseFloat()
    {
        Node* sign = ParseSign();

        if(sign == NULL)
        {
            
            // Parse first set of digits
            Node* digits1 = ParseDigits();
            if(digits1 == NULL) return NULL;

            // Match for decimal point
            if(!Match(TokenType::Fullstop))
            {
                PreviousToken(); //Backtrack to the beginning
                return NULL;
            }

            // Match for digits after the decimal point
            Node* digits2 = ParseDigits();
            if(digits2 == NULL)
            {
                PreviousToken(2); //Backtrack to the beginning
                return NULL;
            }

            Node* floatNode = new Node(Node::Type::Float);
            floatNode->children.push_back(digits1);
            floatNode->children.push_back(digits2);
            DEBUG("Float\n");
            return floatNode;
        }

        else
        {
            // Parse first set of digits
            Node* digits1 = ParseDigits();
            if(digits1 == NULL) 
            {
                DeleteNode(sign);
                PreviousToken(); //Backtrack to the beginning
                return NULL;
            }

            // Match for decimal point
            if(!Match(TokenType::Fullstop))
            {
                DeleteNode(sign);
                PreviousToken(2); //Backtrack to the beginning
                return NULL;
            }

            // Match for digits after the decimal point
            Node* digits2 = ParseDigits();
            if(digits2 == NULL)
            {
                DeleteNode(sign);
                PreviousToken(3); //Backtrack to the beginning
                return NULL;
            }

            Node* floatNode = new Node(Node::Type::Float);
            floatNode->children.push_back(sign);
            floatNode->children.push_back(digits1);
            floatNode->children.push_back(digits2);
            DEBUG("Float\n");
            return floatNode;
        }
    }

    // <namespace-name> ::= <identifier> {"::" <identifier>}
    Node* Parser::ParseNamespaceName()
    {
        Node* identifier = ParseIdentifier();
        if(identifier == NULL) return NULL;

        Node* namespaceName = new Node(Node::Type::NamespaceName);
        namespaceName->children.push_back(identifier);

        while(Match(TokenType::ScopeResolution))
        {
            Node* identifier2 = ParseIdentifier();
            if(identifier2 == NULL) 
            {
                ErrorMsg("Unexpected token in namespace name");
                PreviousToken();
                break;
            }
            namespaceName->children.push_back(identifier2);
        }
        DEBUG("Namespace name\n");
        return namespaceName;
    }

    // <function-name> ::= <identifier> {"::" <identifier>}
    Node* Parser::ParseFunctionName()
    {
        Node* identifier = ParseIdentifier();
        if(identifier == NULL) return NULL;

        Node* functionName = new Node(Node::Type::FunctionName);
        functionName->children.push_back(identifier);

        while(Match(TokenType::ScopeResolution))
        {
            Node* identifier2 = ParseIdentifier();
            if(identifier2 == NULL) 
            {
                ErrorMsg("Unexpected token in function name");
                PreviousToken();
                break;
            }
            functionName->children.push_back(identifier2);
        }
        DEBUG("Function name\n");
        return functionName;
    }

    // <type-name> ::= <identifier>
    //     | <namespace-name> "::" <identifier>
    Node* Parser::ParseTypeName()
    {
        Node* name = ParseIdentifier();
        if(name != NULL)
        {
            Node* typeName = new Node(Node::Type::TypeName);
            typeName->children.push_back(name);
            return typeName;
        }

        uint64_t backtrackPtr = tokenCursor;

        name = ParseNamespaceName();
        if(name == NULL) return NULL;

        if(!Match(TokenType::ScopeResolution))
        {
            DeleteNode(name);
            PreviousToken(tokenCursor-backtrackPtr);
            return NULL;
        }

        Node* identifier = ParseIdentifier();
        if(identifier == NULL)
        {
            DeleteNode(name);
            PreviousToken(tokenCursor-backtrackPtr);
            return NULL;
        }

        Node* typeName = new Node(Node::Type::TypeName);
        typeName->children.push_back(name);
        typeName->children.push_back(identifier);
        return typeName;



    }

    // <name> ::= <identifier> {"." <identifier>}
    Node* Parser::ParseName()
    {
        Node* identifier = ParseIdentifier();
        if(identifier == NULL) return NULL;

        Node* name = new Node(Node::Type::Name);
        name->children.push_back(identifier);

        while(Match(TokenType::Fullstop))
        {
            Node* identifier2 = ParseIdentifier();
            if(identifier2 == NULL)
            {
                ErrorMsg("Unexpected token");
                PreviousToken();
                break;
            }
            name->children.push_back(identifier2);
        }
        DEBUG("Name\n");
        return name;
    }

    // <primitive> ::= "int8" | "uint8" 
    //     | "int16" | "uint16" 
    //     | "int32" | "uint32" 
    //     | "int64" | "uint64"
    //     | "float32" | "float64"
    Node* Parser::ParsePrimitive()
    {
        std::string tokenVal = GetToken().value;
        if(
            tokenVal == "int8" || tokenVal == "uint8" ||
            tokenVal == "int16" || tokenVal == "uint16" ||
            tokenVal == "int32" || tokenVal == "uint32" ||
            tokenVal == "int64" || tokenVal == "uint64" ||
            tokenVal == "float32" || tokenVal == "float64"
        )
        {
            Node* primitive = new Node(Node::Type::Primitive);
            primitive->value = tokenVal;
            return primitive;
        }
        return NULL;
    }

    // <literal> ::= <integer> | <float> | <string>
    Node* Parser::ParseLiteral()
    {
        Node* rule = ParseInteger();
        if(rule != NULL)
        {
            Node* literal = new Node(Node::Type::Literal);
            literal->children.push_back(rule);
            DEBUG("Literal\n");
            return literal;
        }

        rule = ParseFloat();
        if(rule != NULL)
        {
            Node* literal = new Node(Node::Type::Literal);
            literal->children.push_back(rule);
            DEBUG("Literal\n");
            return literal;
        }

        rule = ParseString();
        if(rule != NULL)
        {
            Node* literal = new Node(Node::Type::Literal);
            literal->children.push_back(rule);
            DEBUG("Literal\n");
            return literal;
        }

        return NULL;
    }

    // <type-specifier> ::= <primitive> | <type-name>
    Node* Parser::ParseTypeSpecifier()
    {
        Node* specifier = ParsePrimitive();
        if(specifier == NULL) return NULL;

        specifier = ParseTypeName();
        if(specifier == NULL) return NULL;

        Node* typeSpecifier = new Node(Node::Type::TypeSpecifier);
        typeSpecifier->children.push_back(specifier);
        return typeSpecifier;
    }

    // EXPRESSIONS

    // <expression> ::= <assignment> | <function-call>
    Node* Parser::ParseExpression()
    {
        Node* rule = ParseAssignment();
        if(rule != NULL)
        {
            Node* expression = new Node(Node::Type::Expression);
            expression->children.push_back(rule);
            DEBUG("Expression\n");
            return expression;
        }
        else
        {
            rule = ParseFunctionCall();
            if(rule != NULL)
            {
                Node* expression = new Node(Node::Type::Expression);
                expression->children.push_back(rule);
                DEBUG("Expression\n");
                return expression;
            }
        }
        return NULL;
    }

    // <assignment> ::= 
    //      <left-hand-side> <assignment-operator> <conditional-expression>
    Node* Parser::ParseAssignment()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* lhs = ParseLHS();
        if(lhs == NULL) return NULL;

        Node* assignOperator = ParseAssignmentOperator();
        if(assignOperator == NULL)
        {
            DeleteNode(lhs);
            PreviousToken(tokenCursor-backtrackPtr);
            return NULL;
        }

        Node* conditionalExpression = ParseConditionalExpression();
        if(conditionalExpression == NULL)
        {
            DeleteNode(lhs);
            DeleteNode(assignOperator);
            PreviousToken(tokenCursor-backtrackPtr);
            return NULL;
        }

        Node* assignment = new Node(Node::Type::Assignment);
        assignment->children.push_back(lhs);
        assignment->children.push_back(assignOperator);
        assignment->children.push_back(conditionalExpression);
        DEBUG("Assignmet\n");
        return assignment;
    }

    // <assignment-operator> ::= "=" | "+=" | "-=" | "*=" | "/="
    Node* Parser::ParseAssignmentOperator()
    {
        if(GetToken().IsAssignment())
        {
            Node* assignOperator = new Node(Node::Type::AssignmentOperator);
            assignOperator->value = GetToken().value;
            NextToken();
            DEBUG("Assign operator\n");
            return assignOperator;
        }

        return NULL;
    }

    // <left-hand-side> ::= <name> | <array-access>
    Node* Parser::ParseLHS()
    {
        Node* lhs = ParseName();
        if(lhs != NULL)
        { 
            Node* expressionLHS = new Node(Node::Type::LHS);
            expressionLHS->children.push_back(lhs);
            DEBUG("Expression LHS\n");
            return expressionLHS;
        }
        else
        {
            lhs = ParseArrayAccess();
            if(lhs!= NULL)
            {
                Node* expressionLHS = new Node(Node::Type::LHS);
                expressionLHS->children.push_back(lhs);
                DEBUG("Expression LHS\n");
                return expressionLHS;
            }
        }
        return NULL;
    }

    Node* Parser::ParseArrayAccess()
    {
        return NULL;
    }

    // <conditional-expression> ::= 
    //     <logical-expression> {<conditional-operator> <logical-expression>}

    Node* Parser::ParseConditionalExpression()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* logicalExpression = ParseLogicalExpression();
        if(logicalExpression == NULL) return NULL;

        Node* conditionalExpression = 
            new Node(Node::Type::ConditionalExpression);

        conditionalExpression->children.push_back(logicalExpression);

        for(;;)
        {
            Node* conditionalOperator = ParseConditionalOperator();
            if(conditionalOperator == NULL) break;

            Node* logicalExpression2 = ParseLogicalExpression();
            if(logicalExpression2 == NULL)
            {
                ErrorMsg("Invalid Syntax");
                PreviousToken(tokenCursor-backtrackPtr);
                return NULL;
            }

            conditionalExpression->children.push_back(conditionalOperator);
            conditionalExpression->children.push_back(logicalExpression2);
        }

        DEBUG("Conditional expression\n");
        return conditionalExpression;
    }

    // <conditional-operator> ::= "||" | "&&"
    Node* Parser::ParseConditionalOperator()
    {
        if(GetToken().value == "||" || GetToken().value == "&&")
        {
            Node* op = new Node(Node::Type::ConditionalOperator);
            op->value = GetToken().value;
            NextToken();
            return op;
        }
        return NULL;
    }

    // <logical-expression> ::= <comparison-expression>
    //     | <logical-expression> <logical-operator> <comparison-expression>
    Node* Parser::ParseLogicalExpression()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* expression = ParseComparisonExpression();
        if(expression != NULL)
        {
            Node* logicalExpression = new Node(Node::Type::LogicalExpression);
            logicalExpression->children.push_back(expression);
            DEBUG("Logical expression\n");
            return logicalExpression;
        }
        else
        {
            expression = ParseLogicalOperator();
            if(expression == NULL) return NULL;

            Node* op = ParseLogicalExpression();
            if(op == NULL)
            {
                DeleteNode(expression);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* expression2 = ParseComparisonExpression();
            if(expression2 == NULL)
            {
                DeleteNode(expression);
                DeleteNode(op);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* logicalExpression = new Node(Node::Type::LogicalExpression);
            logicalExpression->children.push_back(expression);
            logicalExpression->children.push_back(op);
            logicalExpression->children.push_back(expression2);
            DEBUG("Logical expression\n");
            return logicalExpression;
        }

        return NULL;
    }


    // <logical-operator> ::= "&" | "|" | "^"
    Node* Parser::ParseLogicalOperator()
    {
        if(
            GetToken().value == "&" || 
            GetToken().value == "|" ||
            GetToken().value == "^"
        )
        {
            Node* op = new Node(Node::Type::LogicalOperator);
            op->value = GetToken().value;
            NextToken();
            return op;
        }
        return NULL;
    }

    // <comparison-expression> ::= <shift-expression>
    //      | <comparison-expression> <comparison-operator> <shift-expression>
    Node* Parser::ParseComparisonExpression()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* expression = ParseShiftExpression();
        if(expression != NULL)
        {
            Node* comparisonExpression 
                = new Node(Node::Type::ComparisonExpression);
            comparisonExpression->children.push_back(expression);
            DEBUG("Comparison expression\n");
            return comparisonExpression;
        }
        else
        {
            expression = ParseComparisonExpression();
            if(expression == NULL) return NULL;

            Node* op = ParseComparisonOperator();
            if(op == NULL)
            {
                DeleteNode(expression);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* expression2 = ParseShiftExpression();
            if(expression2 == NULL)
            {
                DeleteNode(expression);
                DeleteNode(op);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* comparisonExpression 
                = new Node(Node::Type::ComparisonExpression);
            comparisonExpression->children.push_back(expression);
            comparisonExpression->children.push_back(op);
            comparisonExpression->children.push_back(expression2);
            DEBUG("Comparison expression\n");
            return comparisonExpression;
        }

        return NULL;
    }
    
    // <comparison-operator> ::= "==" | "!=" | "<=" | ">="
    Node* Parser::ParseComparisonOperator()
    {
        if(
            GetToken().value == "==" || 
            GetToken().value == "!=" ||
            GetToken().value == "<=" ||
            GetToken().value == ">="
        )
        {
            DEBUG("Comparison Operator")
            Node* op = new Node(Node::Type::ComparisonOperator);
            op->value = GetToken().value;
            NextToken();
            return op;
        }
        return NULL;
    }

    // <shift-expression> ::= <addition>
    //     | <shift-expression> <shift-operator> <addition>
    Node* Parser::ParseShiftExpression()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* expression = ParseAddition();
        if(expression != NULL)
        {
            Node* shiftExpression = new Node(Node::Type::ShiftExpression);
            shiftExpression->children.push_back(expression);
            DEBUG("Shift expression\n");
            return shiftExpression;
        }
        else
        {
            expression = ParseShiftExpression();
            if(expression == NULL) return NULL;

            Node* op = ParseShiftOperator();
            if(op == NULL)
            {
                DeleteNode(expression);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* expression2 = ParseAddition();
            if(expression2 == NULL)
            {
                DeleteNode(expression);
                DeleteNode(op);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* shiftExpression = new Node(Node::Type::ShiftExpression);
            shiftExpression->children.push_back(expression);
            shiftExpression->children.push_back(op);
            shiftExpression->children.push_back(expression2);
            DEBUG("Shift expression\n");
            return shiftExpression;
        }

        return NULL;
    }
    
    // <shift-operator> ::= "<<" | ">>"
    Node* Parser::ParseShiftOperator()
    {
        if(GetToken().value == "<<" || GetToken().value == ">>")
        {
            Node* op = new Node(Node::Type::ShiftOperator);
            op->value = GetToken().value;
            NextToken();
            return op;
        }
        return NULL;
    }

    // <addition> ::= <multiplication>
    //     | <addition> <addition-operator> <multiplication>
    Node* Parser::ParseAddition()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* expression = ParseMultiplication();
        if(expression != NULL)
        {
            Node* addition = new Node(Node::Type::Addition);
            addition->children.push_back(expression);
            DEBUG("Addition\n");
            return addition;
        }
        else
        {
            expression = ParseAddition();
            if(expression == NULL) return NULL;

            Node* op = ParseAdditionOperator();
            if(op == NULL)
            {
                DeleteNode(expression);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* expression2 = ParseMultiplication();
            if(expression2 == NULL)
            {
                DeleteNode(expression);
                DeleteNode(op);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* addition = new Node(Node::Type::Addition);
            addition->children.push_back(expression);
            addition->children.push_back(op);
            addition->children.push_back(expression2);
            DEBUG("Addition\n");
            return addition;
        }

        return NULL;
    }
    
    // <addition-operator> ::= "+" | "-"
    Node* Parser::ParseAdditionOperator()
    {
        if(GetToken().value == "+" || GetToken().value == "-")
        {
            Node* op = new Node(Node::Type::AdditionOperator);
            op->value = GetToken().value;
            NextToken();
            return op;
        }
        return NULL;
    }

    // <multiplication> ::= <factor> 
    //     | <multiplication> <multiplication-operator> <factor>
    Node* Parser::ParseMultiplication()
    {
        uint64_t backtrackPtr = tokenCursor;

        Node* expression = ParseFactor();
        if(expression != NULL)
        {
            Node* multiplication = new Node(Node::Type::Multiplication);
            multiplication->children.push_back(expression);
            DEBUG("Multiplication\n");
            return multiplication;
        }
        else
        {
            expression = ParseMultiplication();
            if(expression == NULL) return NULL;

            Node* op = ParseMultiplicationOperator();
            if(op == NULL)
            {
                DeleteNode(expression);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* expression2 = ParseFactor();
            if(expression2 == NULL)
            {
                DeleteNode(expression);
                DeleteNode(op);
                PreviousToken(tokenCursor - backtrackPtr);
                return NULL;
            }

            Node* multiplication = new Node(Node::Type::Multiplication);
            multiplication->children.push_back(expression);
            multiplication->children.push_back(op);
            multiplication->children.push_back(expression2);
            DEBUG("Multiplication\n");
            return multiplication;
        }

        return NULL;
    }
    
    // <multiplication-operator> ::= "*" | "/"
    Node* Parser::ParseMultiplicationOperator()
    {
        if(GetToken().value == "*" || GetToken().value == "/")
        {
            Node* op = new Node(Node::Type::MultiplicationOperator);
            op->value = GetToken().value;
            NextToken();
            return op;
        }
        return NULL;
    }

    // <factor> ::= <literal> | "(" <expression> ")" | <function-call>
    Node* Parser::ParseFactor()
    {
        Node* rule = ParseLiteral();
        if(rule != NULL)
        {
            Node* factor = new Node(Node::Type::Factor);
            factor->children.push_back(rule);
            DEBUG("Factor\n");
            return factor;
        }

        rule = ParseFunctionCall();
        if(rule != NULL)
        {
            Node* factor = new Node(Node::Type::Factor);
            factor->children.push_back(rule);
            DEBUG("Factor\n");
            return factor;
        }

        uint64_t backtrackPtr = tokenCursor;

        if(!Match(TokenType::OpenParam)) return NULL;

        rule = ParseExpression();
        if(rule == NULL)
        {
            PreviousToken();
            return NULL;
        }

        if(!Match(TokenType::OpenParam))
        {
            DeleteNode(rule);
            PreviousToken(tokenCursor-backtrackPtr);
            return NULL;
        }

        Node* factor = new Node(Node::Type::Factor);
        factor->children.push_back(rule);
        DEBUG("Factor\n");
        return factor;
    }
    
    Node* Parser::ParseFunctionCall()
    {
        return NULL;
    }


    // SCOPES

    // <namespace-scope> ::= "{" <namespace-statements> "}" 
    Node* Parser::ParseNamespaceScope()
    {
        if(!Match(TokenType::OpenScope)) return NULL;

        uint64_t backtrackPtr = tokenCursor;
        Node* namespaceStatements = ParseNamespaceStatements();


        if(!Match(TokenType::CloseScope))
        {
            PreviousToken(tokenCursor-backtrackPtr); // Backtrack
            if(namespaceStatements != NULL) delete namespaceStatements;
            return NULL;
        }

        Node* namespaceScope = new Node(Node::Type::NamespaceScope);

        if(namespaceStatements != NULL) 
            namespaceScope->children.push_back(namespaceStatements);
        
        DEBUG("Namespace scope\n");
        return namespaceScope;
    }

    // <namespace-statements> ::= {<namespace-statement>}
    Node* Parser::ParseNamespaceStatements()
    {
        Node* namespaceStatements = new Node(Node::Type::NamespaceStatements);

        for(;;)
        {
            Node* n = ParseNamespaceStatement();
            if(n == NULL) break;
            namespaceStatements->children.push_back(n);
        }

        DEBUG("Namespace statements\n");
        return namespaceStatements;
    }

    // <namespace-statement> ::= <function-declaration> | <type-declaration>
    Node* Parser::ParseNamespaceStatement()
    {
        Node* declaration = ParseFunctionDeclaration();
        // In case of function declaration
        if(declaration != NULL)
        {
            Node* namespaceStatement = new Node(Node::Type::NamespaceStatement);
            namespaceStatement->children.push_back(declaration);
            DEBUG("Namespace statement\n");
            return namespaceStatement;
        }

        else
        {
            // TODO: Parse type declaration
        }

        return NULL;
    }

    // <local-scope> ::= "{" <local-statements> "}"
    Node* Parser::ParseLocalScope()
    {
        uint64_t backtrackPtr = tokenCursor;

        if(!Match(TokenType::OpenScope)) return NULL;
        
        Node* localStatements = ParseLocalStatements();
        
        if(!Match(TokenType::CloseScope))
        {
            DeleteNode(localStatements);
            PreviousToken(tokenCursor - backtrackPtr);
            return NULL;
        }

        Node* localScope = new Node(Node::Type::LocalScope);
        localScope->children.push_back(localStatements);
        DEBUG("Local scope\n");
        return localScope;
    }

    // <local-statements> ::= { <local-statement> ";" }
    Node* Parser::ParseLocalStatements()
    {
        Node* localStatements = new Node(Node::Type::LocalStatements);

        for(;;)
        {
            Node* n = ParseLocalStatement();
            if(n == NULL) break;
            // if(!Match(TokenType::Semicolon))
            // {
            //     ErrorMsg("Expected semicolon");
            //     NextToken();
            // }
            localStatements->children.push_back(n);
        }

        DEBUG("Local statements\n");
        return localStatements;
    }

    // <local-statement> ::= <variable-declaration> | <expression> 
    Node* Parser::ParseLocalStatement()
    {
        Node* statement = ParseVariableDeclaration();
        if(statement != NULL)
        {
            Node* localStatement = new Node(Node::Type::LocalStatement);
            localStatement->children.push_back(statement);
            DEBUG("Local statement\n");
            return localStatement;
        }

        else
        {
            statement = ParseExpression();
            if(statement == NULL) return NULL;
            Node* localStatement = new Node(Node::Type::LocalStatement);
            localStatement->children.push_back(statement);
            DEBUG("Local statement\n");
            return localStatement;
        }
        
        return NULL;
    }


    //TYPES

    //DECLARATIONS

    // <function-declaration> ::= 
    //     "function" [<function-properties>] <function-name> 
    //     "(" [<parameters>] ")" <local-scope>
    Node* Parser::ParseFunctionDeclaration()
    {
        uint64_t backtrackPtr = tokenCursor;
        if(!Match(TokenType::Function)) return NULL;

        // TODO: parse function properties
        Node* functionProperties = ParseFunctionProperties();

        Node* functionName = ParseFunctionName();
        if(functionName == NULL)
        {
            PreviousToken(tokenCursor-backtrackPtr);
            DeleteNode(functionProperties);
            return NULL;
        }

        if(!Match(TokenType::OpenParam))
        {
            PreviousToken(tokenCursor-backtrackPtr);
            DeleteNode(functionProperties);
            return NULL;
        }

        Node* parameters = ParseParameters();

        if(!Match(TokenType::CloseParam))
        {
            PreviousToken(tokenCursor-backtrackPtr);
            DeleteNode(functionProperties);
            DeleteNode(parameters);
            return NULL;
        }
        // if(parameters == NULL)
        // {
        //     PreviousToken(tokenCursor-backtrackPtr);
        //     DeleteNode(functionProperties);
        //     DeleteNode(functionName);
        //     return NULL;
        // }

        Node* localScope = ParseLocalScope();
        if(localScope == NULL)
        {
            PreviousToken(tokenCursor-backtrackPtr);
            DeleteNode(functionProperties);
            DeleteNode(functionName);
            return NULL;
        }

        Node* functionDeclaration = new Node(Node::Type::Function);
        functionDeclaration->children.push_back(localScope);
        DEBUG("Function declaration\n");
        return functionDeclaration;
    }

    // <parameters> ::= <parameter> {"," <parameter>}
    Node* Parser::ParseParameters()
    {
        Node* parameter = ParseParameter();
        if(parameter == NULL) return NULL;

        Node* parameters = new Node(Node::Type::Parameters);
        parameters->children.push_back(parameter);

        while(Match(TokenType::Comma))
        {
            parameter = ParseParameter();
            if(parameter == NULL)
            {
                ErrorMsg("Unexpected token");
                PreviousToken();
                break;
            }
            parameters->children.push_back(parameter);
        }
        DEBUG("Parameters\n");
        return parameters;
    }

    // <parameter> ::= <type-specifier> <identifier>
    Node* Parser::ParseParameter()
    {
        uint64_t backtrackPtr = tokenCursor;
        Node* type = ParseTypeSpecifier();
        if(type == NULL) return NULL;

        Node* identifier = ParseIdentifier();
        if(identifier == NULL)
        {
            DeleteNode(type);
            PreviousToken(tokenCursor-backtrackPtr);
            return NULL;
        }

        Node* parameter = new Node(Node::Type::Parameter);
        parameter->children.push_back(type);
        parameter->children.push_back(identifier);
        DEBUG("Parameter\n");
        return parameter;
    }

    Node* Parser::ParseFunctionProperties()
    {
        return NULL;
    }

    Node* Parser::ParseVariableDeclaration()
    {
        return NULL;
    }


    // <namespace-declaration> ::= 
    //      "namespace" <namespace-name> <namespace-scope>
    Node* Parser::ParseNamespaceDeclaration()
    {
        DEBUG("Namespace declaration\n");

        uint64_t backtrackPtr = tokenCursor;
        if(!Match(TokenType::Namespace)) return NULL;

        Node* nn = ParseNamespaceName();
        if(nn == NULL) 
        {
            ErrorMsg("Syntax Error");
            PreviousToken(tokenCursor-backtrackPtr+1); // Backtrack
            return NULL;
        }

        Node* ns = ParseNamespaceScope();
        if(ns == NULL) 
        {
            ErrorMsg("Syntax Error");
            PreviousToken(tokenCursor-backtrackPtr+1); // Backtrack
            return NULL;
        }

        Node* namespaceDeclaration = new Node();
        namespaceDeclaration->type = Node::Type::Namespace;
        namespaceDeclaration->children.push_back(nn);
        namespaceDeclaration->children.push_back(ns);

        return namespaceDeclaration;
    }

}