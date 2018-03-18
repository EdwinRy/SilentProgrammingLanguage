#include "SilentParser.hpp"
using namespace SilentTokenizer; 
//using namespace SilentParser;
namespace SilentParser
{
    //Parser state
    bool errorsThrown = false;
    unsigned int errorCount = 0;

    //Allow access to those variables throughout the namespace
    //Global scope reference
    silentProgram *globalScope;
    //Variable found in search
    silentVariable foundVar;
    //Currently parsed function
    bool useCurrentFunction;

    //Scope information
    //Variables currently in scope
    std::vector<silentVariable> scopeVars;

    bool inFunction = false;
    silentFunction* currentFunction;
    bool inLoop = false;

    void errorMessage(std::string message, unsigned int line)
    {
        printf("Error: %s - on line %u\n",message.data(),line);
        errorsThrown = true;
        errorCount += 1;
    }

    void warningMessage(std::string message)
    {
        printf("Warning: %s\n",message.data());
    }

    //Find the variable in the closest scope
    char getVariable(silentToken var)
    {
        //Scan through the global scope
        for(unsigned int i = 0; i < globalScope->globals.size(); i++)
        {
            //If it was found
            if( globalScope->globals[i].name == var.value)
            {
                //Assign to found variable and return
                foundVar = globalScope->globals[i];
                return 0;
            }
        }
        //Scan through the accessible scope
        for(unsigned int i = 0; i < scopeVars.size(); i++)
        {
            //If the variable is found
            if(scopeVars[i].name == var.value)
            {
                //Assign the found variable and return
                foundVar = scopeVars[i];
                return 1;
            }
        }
        errorMessage("Variable "+var.value+" was not found",var.currentLine);
        return -1;
    }

    //Check whether a function exists
    bool checkExistingFunction(std::string name)
    {
        for(unsigned int i = 0; i < globalScope->functions.size();i++)
        {
            if(name == globalScope->functions[i].name)
            {
                return true;
            }
        }
        return false;
    }

    //Get size (in bytes) of a type
    unsigned int getTypeSize(std::string type)
    {
        if(type == "int")
        {
            return 4;
        }
        else if(type == "long")
        {
            return 8;
        }
        else if(type == "float")
        {
            return 4;
        }
        else if(type == "double")
        {
            return 8;
        }
        else if(type == "string")
        {
            return 8;
        }
        else{
            for(unsigned int i = 0; i < globalScope->structures.size();i++)
            {
                if(type == globalScope->structures[i].name)
                {
                    return globalScope->structures[i].size;
                }
            }
        }
        return 0;
    }

    //Get data type based on string
    silentDataType getDataType(silentToken type)
    {
        if(type.type == silentTypeToken)
        {
            if(type.value == "int")
            {
                return silentIntType;
            }
            else if(type.value == "long")
            {
                return silentLongType;
            }
            else if(type.value == "float")
            {
                return silentFloatType;
            }
            else if(type.value == "double")
            {
                return silentDoubleType;
            }
            else if(type.value == "string")
            {
                return silentStringType;
            }
            else{
                return silentNullType;
            }
        }
        else
        {
            //Iterate through global scope structures
            for(unsigned int i = 0; i < globalScope->structures.size();i++)
            {
                if(type.value == globalScope->structures[i].name)
                {
                   return silentStructType;
                }
            }
            errorMessage("Use of incorrect type "+type.value,
                type.currentLine
            );
            return silentNullType;
        }
    }

    //Get name for the identifier
    std::string getIdentifierName(std::vector<silentToken> tokens, int *index)
    {
        if(tokens[*index].type != silentIdentifierToken)
        {
            errorMessage(
                "Incorrect token " + tokens[*index].value + " (expected identifier)",
                tokens[*index].currentLine
            );
        }
        *index+=1;
        return(tokens[*index-1].value);
    }

    //Prepare expression to be parsed more easily
    std::vector<silentToken> prepareExpression(
        std::vector<silentToken> tokens, int *index)
    {
        //Output expression
        std::vector<silentToken> expression;
        //Declare sample tokens
        silentToken openParenthese;
        openParenthese.type = silentOpenBracketToken;
        openParenthese.value = "(";
        silentToken closeParenthese;
        closeParenthese.type = silentClosingBracketToken;
        closeParenthese.value = ")";
        silentToken plus;
        plus.type = silentMathsOperatorToken;
        plus.value = "+";

        //Copy the raw expression string
        while(tokens[*index].value != ";" && tokens[*index].value != ")")
        {
            expression.push_back(tokens[*index]);
            *index+=1;
        }
        expression.push_back(tokens[*index]);
        *index+=1;

        //Simplify expressions
        for(unsigned int i = 0; i < expression.size();i++)
        {
            if(expression[i].value == "-")
            {
                if(expression[i+1].value == "-")
                {
                    expression.erase(expression.begin()+i);
                    expression.erase(expression.begin()+i);
                    expression.insert(expression.begin()+i, plus);
                    i = 0; continue;
                }
                else if(expression[i+1].value == "+")
                {
                    expression.erase(expression.begin()+i+1);
                    i = 0; continue;
                }
            }
            else if(expression[i].value == "+")
            {
                if(expression[i+1].value == "+" || expression[i+1].value == "-")
                {
                    expression.erase(expression.begin()+i);
                    i = 0; continue;
                }
            }
        }

        //Separate out most important operations first
        for(unsigned int i = 0; i < expression.size();i++)
        {   
            if(expression[i].value == "*" || expression[i].value == "/")
            {            
                //Simplify left side
                if(expression[i-1].value != ")")
                {
                    expression.insert(expression.begin()+i-1,openParenthese);
                    i+=1;
                }
                else
                {
                    unsigned int j;
                    unsigned int bracketDegree = 1;
                    for(j = i-2; bracketDegree != 0; j--)
                    {
                        if(expression[j].value == ")")
                        {
                            bracketDegree += 1;
                        }
                        else if(expression[j].value == "(")
                        {
                            bracketDegree -= 1;
                        }
                    }
                    expression.insert(expression.begin()+j+1,openParenthese);
                    i++;
                }
                //Simplify right side
                if(expression[i+1].value != "(")
                {
                    expression.insert(expression.begin()+i+2,closeParenthese);
                    i+=1;
                }
                else
                {
                    unsigned int j;
                    unsigned int bracketDegree = 1;
                    for(j = i+2; bracketDegree != 0; j++)
                    {
                        if(expression[j].value == ")")
                        {
                            bracketDegree -= 1;
                        }
                        else if(expression[j].value == "(")
                        {
                            bracketDegree += 1;
                        }
                    }
                    expression.insert(expression.begin()+j-1,closeParenthese);
                    i++;
                }
            }
            else if (expression[i].value == "=" || expression[i].value == "return")
            {
                expression.insert(expression.end()-1, closeParenthese);
                expression.insert(expression.begin()+i+1, openParenthese);
            }
        }

        //Separate out less important maths operations
        for(unsigned int i = 0; i < expression.size();i++)
        {   
            if(expression[i].value == "+" || expression[i].value == "-")
            {            
                //Simplify left side
                if(expression[i-1].value != ")")
                {
                    expression.insert(expression.begin()+i-1,openParenthese);
                    //
                    i+=1;
                }
                else
                {
                    unsigned int j;
                    unsigned int bracketDegree = 1;
                    for(j = i-2; bracketDegree != 0; j--)
                    {
                        if(expression[j].value == ")")
                        {
                            bracketDegree += 1;
                        }
                        else if(expression[j].value == "(")
                        {
                            bracketDegree -= 1;
                        }
                    }
                    expression.insert(expression.begin()+j+1,openParenthese);
                    i++;
                }
                //Simplify right side
                if(expression[i+1].value != "(")
                {
                    expression.insert(expression.begin()+i+2,closeParenthese);
                    i+=1;
                }
                else
                {
                    unsigned int j;
                    unsigned int bracketDegree = 1;
                    for(j = i+2; bracketDegree != 0; j++)
                    {
                        if(expression[j].value == ")")
                        {
                            bracketDegree -= 1;
                        }
                        else if(expression[j].value == "(")
                        {
                            bracketDegree += 1;
                        }
                    }
                    expression.insert(expression.begin()+j-1,closeParenthese);
                    i++;
                }
            }
        }
        return expression;
    }

    //Parse expression recursively and output intermediate code
    void parseExpression(
        std::vector<silentToken> expressionStr,
        int *index,
        std::vector<std::string> *expression,
        silentDataType expectedType
    )
    {
        for(;expressionStr[*index].value != ")" && 
            expressionStr[*index].value != ","; *index+=1)
        {
            //Parse priority operation
            if(expressionStr[*index].value == "(")
            {
                printf("expression\n");
                *index += 1;
                int saveIndex = *index - 2;
                parseExpression(expressionStr,index,expression,expectedType);
                if(expressionStr[saveIndex].type == silentMathsOperatorToken)
                {
                    printf("math operator %s\n",
                        expressionStr[saveIndex].value.data());
                    expression->push_back(expressionStr[saveIndex].value);
                }
            }
            //Parse number
            else if(expressionStr[*index].type == silentNumberToken)
            {
                printf("token %s\n",expressionStr[*index].value.data());
                expression->push_back(
                    "pushNum " + expressionStr[*index].value
                );
                if(expressionStr[*index -1].type == silentMathsOperatorToken)
                {
                    printf("math operator %s\n",
                        expressionStr[*index -1].value.data());
                    expression->push_back(
                        expressionStr[*index -1].value
                    );
                }
            }
            //Parse identifier
            else if(expressionStr[*index].type == silentIdentifierToken)
            {
                //If the identifier is a function
                if(expressionStr[*index + 1].value == "(")
                {

                }
                //If the identifier is a variable
                printf("token %s\n",expressionStr[*index].value.data());
                expression->push_back(
                    "pushVar " + expressionStr[*index].value
                );
                if(expressionStr[*index -1].type == silentMathsOperatorToken)
                {
                    expression->push_back(
                        expressionStr[*index -1].value
                    );
                }
            }
        }
        printf("End operation\n");
    }


    //Parse global variable declaration
    silentVariable parseVarDeclaration(std::vector<silentToken> tokens, int *index, 
        unsigned int varIndex, std::vector<std::string> *expression)
    {
        silentVariable variable;
        variable.scopeIndex = varIndex;
        silentExpression outputExpression;
        //Get variable type
        *index+=1;
        variable.dataType = getDataType(tokens[*index]);
        variable.size = getTypeSize(tokens[*index].value);

        //Get variable name
        *index+=1;
        if(tokens[*index].type == silentIdentifierToken)
        {
            variable.name = tokens[*index].value;
        }
        else
        {
            errorMessage(
                "Expected variable name in place of \""+tokens[*index].value +"\"",
                tokens[*index].currentLine
            );
        }
        //Check whether it's been initialised
        *index+=1;
        if(tokens[*index].value == ";") ;

        else if(tokens[*index].value == "=")
        {
            int eIndex = 2;
            std::vector<silentToken> expressionStr = prepareExpression(tokens,index);
            parseExpression(expressionStr, &eIndex, expression, variable.dataType);
        }

        else
        {
            errorMessage("Incorrect token after variable declaration",
                tokens[*index].currentLine
            );
        }
        return variable;

    }

    //Parse structure declaration
    silentStructure parseStruct(std::vector<silentToken> tokens, int *index)
    {
        silentStructure structure;
        structure.size = 0;

        //Get structure name
        *index+=1;
        structure.name = getIdentifierName(tokens,index);
        if(tokens[*index].value != "{")
        {
            printf("expected struct declaration on line %u\n",
                tokens[*index].currentLine);
            exit(1);
        }
        *index+=1;
        while(tokens[*index].value != "}")
        {
            silentVariable variable;

            //Get variable type
            *index+=1;
            variable.dataType = getDataType(tokens[*index]);
            if(tokens[*index].value == structure.name)
            {
                errorMessage(
                    "Structure member type can't be the same as "
                    "the currently declared structure",
                    tokens[*index].currentLine
                );
            }     
            variable.size = getTypeSize(tokens[*index].value);

            //Get variable name
            *index+=1;
            if(tokens[*index].type == silentIdentifierToken)
            {
                variable.name = tokens[*index].value;
            }
            else
            {
                printf("Expected variable name in place of \"%s\" on line %i\n",
                        tokens[*index].value.data(),tokens[*index].currentLine);
                exit(1);
            }
            //Check whether it's been initialised
            *index+=1;
            if(tokens[*index].value != ";")
            {
                printf("Expected a \";\" at the end of line %i (no declaraction allowed)\n",
                            tokens[*index].currentLine);
                exit(1);
            }

            structure.variables.push_back(variable);
            structure.size += variable.size;
            *index+=1;
        }
        return structure;
    }

    
    void parseArguments(std::vector<silentToken> tokens, int *index,
        std::vector<std::string> *output, std::string functionName
    )
    {
        printf("func %s\n",functionName.data());
        unsigned int i;
        bool found = false;
        for(i = 0; i < globalScope->functions.size();i++)
        {
            if(globalScope->functions[i].name == functionName)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            printf("Function %s doesn't exist yet it was called on line %i\n",
                functionName.data(),tokens[*index].currentLine
            );
            exit(1);
        }
        silentFunction function = globalScope->functions[i];

        std::vector<silentToken> expression = prepareExpression(tokens,index);
        int eIndex = 2;
        for(; expression[eIndex-1].value != ")"; eIndex++)
        {
        
            parseExpression(expression,&eIndex,output,
                function.arguments[i].dataType
            );
            printf("here\n");
            printf("val %s\n",expression[eIndex].value.data());

            std::vector<std::string> ou = *output;
            for(unsigned int j = 0; j < ou.size(); j++)
            {
                printf("k %s\n",ou[j].data());
            }
        }
        *index-=2;
    }

    void parseScope(
        std::vector<silentToken> tokens, 
        int *index, unsigned int *varIndex,
        silentScope *scope
    )
    {
        //Check whether scope should be declared
        if(tokens[*index].value != "{")
        {
            errorMessage("Expected scope declaration", tokens[*index].currentLine);
        }
        //Parse scope
        *index+=1;
        unsigned int scopeCountBefore = scopeVars.size();
        for(;tokens[*index].value != "}"; *index+=1)
        {
            switch(tokens[*index].type)
            {
                //Parse structure declarations
                case silentStructureToken:
                    //Parse variable declaration
                    if(tokens[*index].value == "var")
                    {
                        //Parse variable
                        silentVariable newVar = parseVarDeclaration(
                            tokens,index,*varIndex,&scope->expressions
                        );
                        //Add to scope variables
                        scope->variables.push_back(newVar);
                        //Add to accessible variables
                        scopeVars.push_back(newVar);

                        //Store variable in memory
                        scope->expressions.push_back(
                            "alloc"+std::to_string(newVar.size)+
                            " i"+std::to_string(newVar.scopeIndex)
                        );
                        scope->expressions.push_back(
                            "store"+std::to_string(newVar.size)+
                            " i"+std::to_string(newVar.scopeIndex)
                        );
                        *varIndex += 1;
                    }
                    else if(tokens[*index].value == "return")
                    {
                        if(inFunction)
                        {
                            int eIndex = 2;
                            std::vector<silentToken> expressionStr;
                            expressionStr = prepareExpression(tokens,index);
                            parseExpression(
                                expressionStr, 
                                &eIndex, 
                                &scope->expressions,
                                currentFunction->returnType
                            );
                            scope->expressions.push_back("ret");
                            *index -= 1;
                        }
                        else
                        {
                            errorMessage(
                                "Return operation prohibited outside of a function", 
                                tokens[*index].currentLine
                            );
                        }
                        
                    }
                    else
                    {
                        errorMessage(
                            "Declaration of structures within this scope prohibited",
                            tokens[*index].currentLine
                        );
                    }
                break;
                //Parse identifier operation
                case silentIdentifierToken:
                    //Parse variable initialisation
                    if(tokens[*index+1].value == "=")
                    {
                        char varSearch = getVariable(tokens[*index]);
                        if(!varSearch)
                        {
                            //Use global
                            scope->expressions.push_back("ug");
                        }
                        int eIndex = 3;
                        std::vector<silentToken> expressionStr = 
                            prepareExpression(tokens,index);

                        parseExpression(expressionStr,&eIndex,
                            &scope->expressions,foundVar.dataType);

                        scope->expressions.push_back(
                            "store"+std::to_string(foundVar.size)+
                            " i"+std::to_string(foundVar.scopeIndex)
                        );                            
                        if(!varSearch)
                        {
                            //End global
                            scope->expressions.push_back("eg");
                        }
                        *index-=1;
                    }
                    //Parse function call
                    else if(tokens[*index+1].value == "(")
                    {
                        int x = *index;
                        //parse arguments
                        parseArguments(tokens,index,&scope->expressions,
                            tokens[*index].value
                        );
                        scope->expressions.push_back("call" + tokens[x].value);
                        printf("here chef\n");
                        if(tokens[*index+=2].value != ";")
                        {
                            errorMessage("Invalid token after function call on line",
                                tokens[*index].currentLine
                            );
                        }
                    }
                    else
                    {
                        errorMessage("Invalid identifier operation",
                            tokens[*index].currentLine
                        );
                    }
                break;
                default:
                    errorMessage(
                        "Invalid token within scope " + tokens[*index].value,
                        tokens[*index].currentLine
                    );
                break;
            }
        }
        unsigned int scopeVarsRemove = scopeVars.size() - scopeCountBefore;
        for(unsigned int i = 0; i < scopeVarsRemove;i++) scopeVars.pop_back(); 
    }

    silentFunction parseFunction(std::vector<silentToken> tokens, int *index)
    {
        silentFunction function;
        //Get function return type
        *index+=1;
        function.returnType = getDataType(tokens[*index]);
        //Get function name
        *index+=1;
        function.name = getIdentifierName(tokens,index);
        //Parse parameters
        if(tokens[*index].value != "(")
        {
            errorMessage("Expected declaration of function parameters",
                tokens[*index].currentLine
            );
        }
        if(tokens[*index+1].value != ")")
        {
            //Parse parameters
            while(tokens[*index].value != ")")
            {
                silentVariable argument;
                //Get argument type
                *index+=1;
                argument.dataType = getDataType(tokens[*index]);
                argument.size = getTypeSize(tokens[*index].value);

                //Get argument name
                *index+=1;
                argument.name = getIdentifierName(tokens,index);
                //*index+=1;

                if(tokens[*index].value != "," && tokens[*index].value != ")")
                {
                    errorMessage("Expected a \",\" or \")\" (no declaraction allowed)",
                        tokens[*index].currentLine
                    );
                }
                function.arguments.push_back(argument);
                function.varIndex += 1;
            }
        }
        else
        {
            *index+=1;
        }
        //Check whether the scope has been initialised
        *index+=1;
        if(tokens[*index].value != "{")
        {
            printf("Expected scope declaration for function \"%s\" on line %i\n",
                    function.name.data(),tokens[*index].currentLine);
            exit(1);
        }

        inFunction = true;
        //Parse scope
        parseScope(tokens,index,&function.varIndex,&function.scope);
        inFunction = false;
        return function;
    }

    silentProgram *silentParseProgram(std::vector<silentToken> tokens)
    {
        silentProgram* program = new silentProgram();
        globalScope = program;
        program->varIndex = 0;

        for(int i = 0; i < (int)tokens.size();i++)
        {
            switch(tokens[i].type)
            {
                case silentStructureToken:
                    if(tokens[i].value == "var")
                    {
                        silentVariable newVar = 
                            parseVarDeclaration(
                                tokens,
                                &i,
                                program->varIndex,
                                &program->expressions
                            );
                        program->globals.push_back(newVar);
                        program->varIndex += 1;
                        scopeVars.push_back(newVar);
                    }
                    if(tokens[i].value == "struct")
                    {
                        program->structures.push_back(parseStruct(tokens,&i));
                    }
                    if(tokens[i].value == "func")
                    {
                        program->functions.push_back(parseFunction(tokens,&i));                       
                    }
                break;
                default:
                    errorMessage("Invalid token in the global scope \""+
                        tokens[i].value+"\"", tokens[i].currentLine
                    );
                break;
            }
        }
        if(errorsThrown)
        {
            printf("Could not compile!\n");
            printf("Errors thrown: %u\n", errorCount);
            exit(1);
        }
        return program;
    }
}