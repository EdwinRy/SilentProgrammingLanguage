#include "SilentWriter.hpp"
#include <string>
using namespace SilentParser;
namespace SilentCodeGenerator
{
    
    silentVariable foundVar;
    silentFunction currentFunction;
    silentProgram *globalScope;
    bool useCurrentFunction;

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

    std::string compileBytecode(SilentParser::silentProgram program)
    {
        std::string output;

        return output;
    }

    void writeAssemblyExpression(
        std::vector<std::string> *output,
        silentExpression expression
    )
    {
        silentValueType lastValueType;
        std::vector<std::string> expressionStr = expression.expression;
        for(unsigned int i = 0; i < expressionStr.size(); i++)
        {
            if(expressionStr[i].substr(0,7) == "pushNum")
            {
                output->push_back(
                    "push4 i"+expressionStr[i].substr(8)
                );
                lastValueType = silentNumericalValue;
            }

            else if(expressionStr[i].substr(0,7) == "pushVar")
            {
                std::string name = expressionStr[i].substr(8);
                char varType = getVariable(name);
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
                        expressionStr[i].substr(8).data());
                    exit(1);
                }
            }

            else if(expressionStr[i] == "+")
            {
                printf("here\n");
                if(lastValueType == silentNumericalValue)
                {
                    output->push_back("addint");
                }
            }
            else if(expressionStr[i] == "-")
            {
                output->push_back("subint"); 
            }
            else if(expressionStr[i] == "*")
            {
                output->push_back("mulint");
            }
            else if(expressionStr[i] == "/")
            {
                output->push_back("divint");
            }
            else if(expressionStr[i] == "ug")
            {
                output->push_back("useglobal");
            }
            else if(expressionStr[i] == "eg")
            {
                output->push_back("endglobal");
            }
        }
    }

    void writeAssemblyDeclaration(
        std::vector<std::string> *output,
        silentVariable variable,
        silentFunction parent
    )
    {
        currentFunction = parent;
        useCurrentFunction = true;
        writeAssemblyExpression(output, variable.value.value);
        useCurrentFunction = false;
        if(variable.dataType != silentStructType)
        {
            output->push_back("alloc"+std::to_string(variable.size)+
                " i"+std::to_string(variable.scopeIndex)
            );

            output->push_back("store"+std::to_string(variable.size)+
                " i"+std::to_string(variable.scopeIndex)
            );
        }
    }

    void writeAssemblyAssignment(
        std::vector<std::string> *output,
        silentExpression expression,
        silentVariable variable,
        silentFunction parent
    )
    {
        
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
            unsigned int varStack = 0;
            unsigned int returnStack = 0;
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
            for(unsigned int j = 0; j < function.expressions.size(); j++)
            {
                //If declaring a variable
                if(function.expressions[j] == "var")
                {
                    writeAssemblyDeclaration(
                        &output,
                        function.variables[varStack],
                        function
                    );
                    varStack += 1;
                }

                //If returning a value
                else if(function.expressions[j] == "ret")
                {
                    writeAssemblyExpression(
                        &output,
                        function.returnValues[returnStack].value
                    );
                    output.push_back("return");
                    returnStack += 1;
                }
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