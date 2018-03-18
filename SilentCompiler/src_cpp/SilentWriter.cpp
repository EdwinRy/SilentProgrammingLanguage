#include "SilentWriter.hpp"
#include <string>
using namespace SilentParser;
namespace SilentCodeGenerator
{
    silentVariable foundVar;
    silentFunction currentFunction;
    silentProgram *globalScope;
    bool useCurrentFunction;

    /*
    char getVariable(std::string name)
    {
        if(useCurrentFunction)
        {
            for(unsigned int i = 0; i < currentFunction.variables.size(); i++)
            {
                if(currentFunction.variables[i].name == name)
                {
                    foundVar = currentFunction.variables[i];
                    return 1;
                }
            }
            for(unsigned int i = 0; i < globalScope->globals.size(); i++)
            {
                if( globalScope->globals[i].name == name)
                {
                    foundVar = globalScope->globals[i];
                    return 2;
                }
            }
        }
        else
        {
            for(unsigned int i = 0; i < globalScope->globals.size(); i++)
            {
                if( globalScope->globals[i].name == name)
                {
                    foundVar = globalScope->globals[i];
                    return 2;
                }
            }
        }
        return 0;
    }
    */

    std::string compileBytecode(SilentParser::silentProgram program)
    {
        std::string output;

        return output;
    }

    void writeAssemblyExpression(
        std::vector<std::string> *output,
        std::vector<std::string> expression
    )
    {
        silentValueType lastValueType;;
        for(unsigned int i = 0; i < expression.size(); i++)
        {
            if(expression[i].substr(0,7) == "pushNum")
            {
                output->push_back(
                    "push4 i"+expression[i].substr(8)
                );
                lastValueType = silentNumericalValue;
            }

            else if(expression[i].substr(0,7) == "pushVar")
            {
                std::string name = expression[i].substr(8);
                char varType = 0;
                if(varType > 0)
                {
                    if(varType == 2)
                    {
                        output->push_back("useglobal");
                    }
                    switch(foundVar.dataType)
                    {
                        case silentFloatType:
                        case silentIntType:
                            output->push_back("load4 i"+
                                std::to_string(foundVar.scopeIndex)
                            );
                        break;
                        case silentDoubleType:
                        case silentLongType:
                            output->push_back("load8 i"+
                                std::to_string(foundVar.scopeIndex)
                            );
                        break;
                        default:
                        printf("Variable of incorrect type using in expression\n");
                            exit(1);
                        break;
                    }
                    if(varType == 2)
                    {
                        output->push_back("endglobal");
                    }
                }
                else
                {
                    printf("Error: invalid token used in expression: %s\n",
                        expression[i].substr(8).data());
                    exit(1);
                }
            }

            else if(expression[i] == "+")
            {
                printf("here\n");
                if(lastValueType == silentNumericalValue)
                {
                    output->push_back("addint");
                }
            }
            else if(expression[i] == "-")
            {
                output->push_back("subint"); 
            }
            else if(expression[i] == "*")
            {
                output->push_back("mulint");
            }
            else if(expression[i] == "/")
            {
                output->push_back("divint");
            }
            else if(expression[i] == "ug")
            {
                output->push_back("useglobal");
            }
            else if(expression[i] == "eg")
            {
                output->push_back("endglobal");
            }
            else if(expression[i] == "ret")
            {
                output->push_back("return");
            }
            else
            {
                output->push_back(expression[i]);
            }
        }
    }

    std::vector<std::string> compileAssembly(SilentParser::silentProgram program)
    {
        std::vector<std::string> output;
        useCurrentFunction = false;
        globalScope = &program;
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
            //writeAssemblyExpression(&output,function.expressions);
        }
        return output;
    }

    std::vector<std::string> compileLibrary(SilentParser::silentProgram program)
    {
        std::vector<std::string> output;


        return output;
    }
}