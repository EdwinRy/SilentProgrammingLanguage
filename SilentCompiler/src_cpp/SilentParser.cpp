#include "SilentParser.hpp"
using namespace SilentTokenizer; 
//using namespace SilentParser;
namespace SilentParser
{
    //Allow access to those variables throughout the namespace
    silentProgram *globalScope;
    silentVariable foundVar;
    silentFunction currentFunction;
    bool useCurrentFunction;

    //Find the variable in the closest scope
    char getVariable(std::string name)
    {
        //If it's referenced from a function
        if(useCurrentFunction)
        {
            //Scan through the function scope
            for(unsigned int i = 0; i < currentFunction.variables.size(); i++)
            {
                //If the variable is found
                if(currentFunction.variables[i].name == name)
                {
                    //Assign the found variable and return
                    foundVar = currentFunction.variables[i];
                    return 1;
                }
            }
            //Scan through the global scope
            for(unsigned int i = 0; i < globalScope->globals.size(); i++)
            {
                //If it was found
                if( globalScope->globals[i].name == name)
                {
                    //Assign to found variable and return
                    foundVar = globalScope->globals[i];
                    return 2;
                }
            }
        }
        //If it shouldn't be searched in the global scope
        else
        {
            //Scan through the global scope
            for(unsigned int i = 0; i < globalScope->globals.size(); i++)
            {
                //If it was found
                if( globalScope->globals[i].name == name)
                {
                    //Assign to found variable and return
                    foundVar = globalScope->globals[i];
                    return 2;
                }
            }
        }
        return 0;
    }

    //Check whether the custom type exists
    bool checkExistingType(std::string type)
    {
        //Iterate through global scope structures
        for(unsigned int i = 0; i < globalScope->structures.size();i++)
        {
            if(type == globalScope->structures[i].name)
            {
                return true;
            }
        }
        return false;
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
    silentDataType getBuiltinDataType(std::string type)
    {
        if(type == "int")
        {
            return silentIntType;
        }
        else if(type == "long")
        {
            return silentLongType;
        }
        else if(type == "float")
        {
            return silentFloatType;
        }
        else if(type == "double")
        {
            return silentDoubleType;
        }
        else if(type == "string")
        {
            return silentStringType;
        }
        else{
            return silentNullType;
        }
    }

    //Parse global variable declaration
    silentVariable parseGlobalVar(std::vector<silentToken> tokens, int *index, 
        unsigned int varIndex)
    {
        silentVariable variable;
        variable.scopeIndex = varIndex;
                            silentExpression outputExpression;
        //Get variable type
        *index+=1;
        if(tokens[*index].type == silentTypeToken)
        {
            variable.dataType = getBuiltinDataType(tokens[*index].value);
        }
        else{
            if(checkExistingType(tokens[*index].value))
            {
                variable.dataType = silentStructType;
            }
            else
            {
                printf("Use of incorrect type \"%s\" on line %i\n",
                    tokens[*index].value.data(),tokens[*index].currentLine);
                exit(1);
            }
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
            printf("Expected a \";\" at the end of line %i (no declaraction allowed),\n",
                        tokens[*index].currentLine);
            exit(1);
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
        if(tokens[*index].type != silentIdentifierToken)
        {
            printf("incorrect token %s on line %u (expected struct name)\n",
                tokens[*index].value.data(),tokens[*index].currentLine);
            exit(1);
        }
        else
        {
            structure.name = tokens[*index].value;
        }
        *index+=1;
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
            if(tokens[*index].type == silentTypeToken)
            {
                variable.dataType = getBuiltinDataType(tokens[*index].value);
            }
            else{
                if(checkExistingType(tokens[*index].value))
                {
                    variable.dataType = silentStructType;
                }
                else if(tokens[*index].value == structure.name)
                {
                    printf("Variable can't have the same type as the struct that it's in\n");
                    printf("error on line %i\n",tokens[*index].currentLine);
                    exit(1);
                }
                else
                {
                    printf("Use of incorrect type \"%s\" on line %i\n",
                        tokens[*index].value.data(),tokens[*index].currentLine);
                    exit(1);
                }
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
        while(tokens[*index].value != ";")
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

    void parseArguments(std::vector<silentToken> tokens, int *index,
        std::vector<std::string> *output, std::string functionName)
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
            //printf("here\n");
            //printf("val %s\n",expression[eIndex].value.data());

            //std::vector<std::string> ou = *output;
            //for(int j = 0; j < ou.size(); j++)
            //{
            //    printf("k %s\n",ou[j].data());
            //}
        }
        *index-=2;
    }

    silentVariable parseFunctionVar(
        silentFunction *function,
        std::vector<silentToken> tokens,
        int *index,
        unsigned int varIndex
    )
    {
        silentVariable variable;
        variable.scopeIndex = varIndex;
        //Get variable type
        *index+=1;
        if(tokens[*index].type == silentTypeToken)
        {
            variable.dataType = getBuiltinDataType(tokens[*index].value);
        }
        else
        {
            if(checkExistingType(tokens[*index].value))
            {
                variable.dataType = silentStructType;
            }

            else
            {
                printf("Use of incorrect type \"%s\" on line %i\n",
                    tokens[*index].value.data(),tokens[*index].currentLine);
                exit(1);
            }
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

        //Check whether it has only been declared
        *index+=1;
        if(tokens[*index].value == ";")
        {
            //variable.value.valueType = silentNullValue;
        }
        //Parse expression if it has been initialised
        else if(tokens[*index].value == "=")
        {
            int eIndex = 2;
            std::vector<silentToken> expressionStr = prepareExpression(tokens,index);
            parseExpression(expressionStr, &eIndex, &function->expressions,
                variable.dataType);
        }
        else
        {
            printf("Incorrect token after variable declaration on line %i\n",
                tokens[*index].currentLine);
        }
        return variable;
    }

    silentFunction parseFunction(std::vector<silentToken> tokens, int *index)
    {
        silentFunction function;
        unsigned int varIndex = 0;
        //Get function return type
        *index+=1;
        if(tokens[*index].type == silentTypeToken)
        {
            function.returnType = getBuiltinDataType(tokens[*index].value);
        }
        else{
            if(checkExistingType(tokens[*index].value))
            {
                function.returnType = silentStructType;
            }
            else
            {
                printf("Use of incorrect type \"%s\" on line %i\n",
                    tokens[*index].value.data(),tokens[*index].currentLine);
                exit(1);
            }
        }

        //Get function name
        *index+=1;
        if(tokens[*index].type == silentIdentifierToken)
        {
            function.name = tokens[*index].value;
        }
        else
        {
            printf("Expected function name in place of \"%s\" on line %i\n",
                tokens[*index].value.data(),tokens[*index].currentLine);
            exit(1);
        }

        //Parse parameters
        *index+=1;
        if(tokens[*index].value != "(")
        {
            printf("Expected arguments for function \"%s\" on line %i\n",
                    function.name.data(),tokens[*index].currentLine);
            exit(1);
        }

        if(tokens[*index+1].value != ")")
        {

            //Parse parameters
            while(tokens[*index].value != ")")
            {
                silentVariable argument;
                //Get argument type
                *index+=1;
                if(tokens[*index].type == silentTypeToken)
                {
                    argument.dataType = getBuiltinDataType(tokens[*index].value);
                }
                else{
                    if(checkExistingType(tokens[*index].value))
                    {
                        argument.dataType = silentStructType;
                    }
                    else
                    {
                        printf("Use of incorrect type \"%s\" on line %i\n",
                            tokens[*index].value.data(),tokens[*index].currentLine);
                        printf("within declaration of function \"%s\"\n",
                            function.name.data());
                        exit(1);
                    }
                }
                argument.size = getTypeSize(tokens[*index].value);

                //Get argument name
                *index+=1;
                if(tokens[*index].type == silentIdentifierToken)
                {
                    argument.name = tokens[*index].value;
                }
                else
                {
                    printf("Expected argument name in place of \"%s\" on line %i\n",
                            tokens[*index].value.data(),tokens[*index].currentLine);
                    exit(1);
                }

                *index+=1;
                if(tokens[*index].value != "," && tokens[*index].value != ")")
                {
                    printf("Expected a \",\" or \")\" on line %i" 
                        "(no declaraction allowed)\n", tokens[*index].currentLine);
                    exit(1);
                }

                function.arguments.push_back(argument);
                varIndex += 1;
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

        //Parse scope
        *index+=1;
        for(;tokens[*index].value != "}"; *index+=1)
        {
            switch(tokens[*index].type)
            {
                //Parse function variable declaration
                case silentStructureToken:
                    //Parse variable declaration
                    if(tokens[*index].value == "var")
                    {
                        silentVariable newVar = 
                            parseFunctionVar(&function, tokens, index,varIndex);
                        function.variables.push_back(newVar);
                        *index -= 1;
                        //function.expressions.push_back("var");
                        function.expressions.push_back(
                            "alloc"+std::to_string(newVar.size)+
                            " i"+std::to_string(newVar.scopeIndex)
                        );

                        function.expressions.push_back(
                            "store"+std::to_string(newVar.size)+
                            " i"+std::to_string(newVar.scopeIndex)
                        );
                        varIndex += 1;
                    }
                    else if(tokens[*index].value == "return")
                    {
                        int eIndex = 2;
                        std::vector<silentToken> expressionStr =
                            prepareExpression(tokens,index);
                        parseExpression(expressionStr, &eIndex, &function.expressions,
                            function.returnType);
                        function.expressions.push_back("ret");
                        *index -= 1;
                    }
                    else
                    {
                        printf("Can't declare structures within function scope,\n");
                        printf("Perhaps you forgot a \"}\" around line %i?\n",
                            tokens[*index].currentLine);
                        exit(1);
                    }                   
                break;
                //Parse function identifier
                case silentIdentifierToken:
                    //Parse assignment
                    if(tokens[*index + 1].value == "=")
                    {
                        useCurrentFunction = true;
                        currentFunction = function;
                        char varSearch = getVariable(tokens[*index].value);
                        if(varSearch > 0)
                        {
                            if(varSearch == 2)
                            {
                                //Use global
                                function.expressions.push_back("ug");
                            }
                            int eIndex = 3;
                            std::vector<silentToken> expressionStr = 
                                prepareExpression(tokens,index);

                            parseExpression(expressionStr,&eIndex,
                                &function.expressions,foundVar.dataType);

                            function.expressions.push_back(
                                "store"+std::to_string(foundVar.size)+
                                " i"+std::to_string(foundVar.scopeIndex)
                            );                            

                            if(varSearch == 2)
                            {
                                //End global
                                function.expressions.push_back("eg");
                            }
                            *index-=1;
                        }
                        else
                        {
                            printf("token \"%s\" found to not be a function or"
                                "a variable",tokens[*index].value.data());
                        }
                        useCurrentFunction = false;
                    }
                    //Parse function call
                    else if(tokens[*index + 1].value == "(")
                    {
                        int x = *index;
                        //parse arguments
                        parseArguments(tokens,index,&function.expressions,
                            tokens[*index].value);
                        function.expressions.push_back("call" + tokens[x].value);
                        printf("here chef\n");
                        
                    }
                    else
                    {
                        printf("Invalid identifier operation on line %i\n",
                            tokens[*index].currentLine);
                    }
                break;
                default:
                    printf("Invalid token in the function scope of \"%s\" on line %i,\n"
                        "Invalid token: \"%s\"\n",
                        function.name.data(), tokens[*index].currentLine,
                        tokens[*index].value.data());
                    exit(1);
                break;
            }
        }
        return function;
    }

    silentProgram *silentParseProgram(std::vector<silentToken> tokens)
    {
        silentProgram* program = new silentProgram();
        globalScope = program;

        unsigned int globalIndex = 0;

        for(int i = 0; i < (int)tokens.size();i++)
        {
            switch(tokens[i].type)
            {
                case silentStructureToken:
                    if(tokens[i].value == "var")
                    {
                        program->globals.push_back(
                            parseGlobalVar(tokens, &i, globalIndex)
                        );
                        globalIndex += 1;
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
                    printf("Invalid token in the global scope \"%s\" on line %i\n",
                        tokens[i].value.data(), tokens[i].currentLine);
                    exit(1);
    
                break;
            }
        }
        return program;
    }
}