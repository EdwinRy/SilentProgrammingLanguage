#include "SilentParser.hpp"
using namespace SilentTokenizer; 
using namespace SilentParser;
namespace SilentParser
{
    silentStructure silentParseStructure(std::vector<silentToken> tokens, int *index)
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

    silentFunction silentParseFunction(std::vector<silentToken> tokens, int *index)
    {
        silentFunction function;

        return function;
    }

    silentProgram *silentParseProgram(std::vector<silentToken> tokens)
    {
        silentProgram* program = new silentProgram();

        for(int i = 0; i < (int)tokens.size();i++)
        {
            switch(tokens[i].type)
            {
                case silentStructureToken:
                    program->functions.push_back(
                        silentParseFunction(tokens,&i)
                    );
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