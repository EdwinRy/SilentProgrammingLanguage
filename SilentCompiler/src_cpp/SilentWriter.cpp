#include "SilentWriter.hpp"
#include <string>
using namespace SilentParser;
namespace SilentCodeGenerator
{
    unsigned int localIndex;
    std::string compileBytecode(SilentParser::silentProgram program)
    {
        std::string output;

        return output;
    }

    std::vector<std::string> compileAssembly(SilentParser::silentProgram program)
    {
        std::vector<std::string> output;

        //write down globals
        for(unsigned int i = 0; i < program.globals.size(); i++)
        {
            if(program.globals[i].dataType == silentStructType)
            {
                output.push_back
                (
                    "allocx i" + std::to_string(program.globals[i].size) +
                    " i" + std::to_string(i)
                );
            }
            else
            {
                output.push_back
                (
                    "alloc"+std::to_string(program.globals[i].size) + " i" +
                    std::to_string(i)
                );
            }
        }


        return output;
    }

    std::vector<std::string> compileLibrary(SilentParser::silentProgram program)
    {
        std::vector<std::string> output;


        return output;
    }
}