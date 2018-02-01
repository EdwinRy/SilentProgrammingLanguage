#include "SilentParser.hpp"
using namespace SilentTokenizer; 
namespace SilentParser
{
    silentStructure silentParseStructure()
    {
        
    }
    silentFunction silentParseFunction()
    {
        silentFunction function;

        return function;
    }

    silentProgram* silentParseProgram(std::vector<silentToken> tokens)
    {
        silentProgram* program = new silentProgram();

        for(unsigned int i = 0; i < tokens.size();i++)
        {
            switch(tokens[i].type)
            {
                case silentStructureToken:
                    program->functions.push_back(
                        silentParseFunction()
                    );
                break;
                default:
                    printf("Invalid token in the global scope ");
                    printf("on line %i\n",tokens[i].currentLine);
                break;
            }
        }
        return program;
    }
}