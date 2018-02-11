#include "SilentParser.hpp"
using namespace SilentTokenizer; 
using namespace SilentParser;
namespace SilentParser
{
    silentProgram* globalScope;
    bool checkExistingType(std::string type)
    {
        for(unsigned int i = 0; i < globalScope->structures.size();i++)
        {
            if(type == globalScope->structures[i].name)
            {
                return true;
            }
        }
        return false;
    }

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
    silentVariable parseGlobalVar(std::vector<silentToken> tokens, int *index)
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

    std::vector<silentToken> prepareExpression(std::vector<silentToken> tokens, int *index)
    {
        std::vector<silentToken> expression;
        silentToken openParenthese;
        openParenthese.type = silentOpenBracketToken;
        openParenthese.value = "(";
        silentToken closeParenthese;
        closeParenthese.type = silentClosingBracketToken;
        closeParenthese.value = ")";

        while(tokens[*index].value != ";")
        {
            expression.push_back(tokens[*index]);
            *index+=1;
        }
        expression.push_back(tokens[*index]);
        *index+=1;
        //while(tokens[*index].value != ";")
        for(unsigned int i = 0; i < expression.size();i++)
        {   
            if(expression[i].type == silentMathsOperatorToken)
            {
                if(expression[i].value == "*" || expression[i].value == "/")
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
                else if (expression[i].value == "=")
                {
                    expression.insert(expression.end()-1, closeParenthese);
                    expression.insert(expression.begin()+i+1, openParenthese);
                }
            }
        }
        return expression;
    }

    silentExpression parseExpression(std::vector<silentToken> expressionString)
    {
        silentExpression expression;
        for(unsigned int i = 0; i < expressionString.size(); i++)
        {
            printf("%s\n",expressionString[i].value.data());
        }
        for(unsigned int i = 0; i < expressionString.size(); i++)
        {
            if(expressionString[i].value == ")")
            {
                //look for the left bracket
                int leftI = i;
                for(int leftI; expressionString[leftI].value == "("; leftI--);

            }
            else if(expressionString[i].value == ";")
            {
                return expression;
            }
        }
        return expression;
    }

    silentFunction parseFunction(std::vector<silentToken> tokens, int *index)
    {
        silentFunction function;

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

            //Parse arguments
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
                        printf("within declaration of function \"%s\"\n",function.name.data());
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
                    printf("Expected a \",\" or \")\" on line %i (no declaraction allowed)\n",
                                tokens[*index].currentLine);
                    exit(1);
                }

                function.arguments.push_back(argument);
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
        while(tokens[*index].value != "}")
        {
            if(tokens[*index].type == silentStructureToken && tokens[*index].value != "var")
            {
                printf("Can't declare structures within function scope,\n");
                printf("Perhaps you forgot a \"}\" around line %i?\n",tokens[*index].currentLine);
                exit(1);
            }

            function.expressions.push_back(
                parseExpression(prepareExpression(tokens,index))
            );
            *index+=1;
        }

        return function;
    }

    silentProgram *silentParseProgram(std::vector<silentToken> tokens)
    {
        silentProgram* program = new silentProgram();
        globalScope = program;

        for(int i = 0; i < (int)tokens.size();i++)
        {
            switch(tokens[i].type)
            {
                case silentStructureToken:
                    if(tokens[i].value == "var")
                    {
                        program->globals.push_back(parseGlobalVar(tokens, &i));
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