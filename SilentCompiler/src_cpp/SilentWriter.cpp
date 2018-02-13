#include "SilentWriter.hpp"
#include <string>
using namespace SilentParser;
namespace SilentCodeGenerator
{
    std::string compileBytecode(SilentParser::silentProgram program)
    {
        std::string output;

        return output;
    }

    std::vector<std::string> compileAssembly(SilentParser::silentProgram program)
    {
        std::vector<std::string> output;

        //Write down globals
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

        //Insert calling main function instruction
        output.push_back("call main");

        //Write down functions
        for(unsigned int i = 0; i < program.functions.size(); i++)
        {
            silentFunction function = program.functions[i];
            unsigned int localIndex = 0;
            //write down name
            output.push_back(function.name + ":");
            //Store arguments
            for(; localIndex < function.arguments.size(); localIndex++)
            {
                if(function.arguments[localIndex].dataType == silentStructType &&
                    function.name != "main")
                {
                    output.push_back
                    (
                        "allocx i" + 
                        std::to_string(function.arguments[localIndex].size) +
                        " i" + std::to_string(localIndex)
                    );
                    output.push_back
                    (
                        "storex i" + 
                        std::to_string(function.arguments[localIndex].size) +
                        " i" + std::to_string(localIndex)
                    );
                }
                else if(function.name != "main")
                {
                    output.push_back
                    (
                        "alloc"+std::to_string(function.arguments[localIndex].size) +
                        " i" + std::to_string(localIndex)
                    );
                    output.push_back
                    (
                        "store"+std::to_string(function.arguments[localIndex].size) +
                        " i" + std::to_string(localIndex)
                    );
                }
                else
                {
                    printf("The main function can't take any arguments\n");
                    exit(1);
                }
            }
            //Write down function's expression
            for(unsigned int j = 0; j < program.functions[i].expressions.size(); j++)
            {
                
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