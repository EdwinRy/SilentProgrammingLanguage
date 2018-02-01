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
        *index+=1;
        //Check for valid type
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

        return variable;
    }
    silentStructure parseStruct(std::vector<silentToken> tokens, int *index)
    {
        silentStructure structure;

        //get name
        *index+=1;
        if(tokens[*index].type != silentIdentifierToken)
        {
            printf("incorrect token %s on line %u",
                tokens[*index].value.data(),tokens[*index].currentLine);
        }

        return structure;
    }

    silentFunction parseFunction(std::vector<silentToken> tokens, int *index)
    {
        silentFunction function;

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
    
                break;
            }
        }
        return program;
    }
}