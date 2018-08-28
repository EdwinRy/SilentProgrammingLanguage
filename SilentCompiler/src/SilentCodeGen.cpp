#include "SilentCodeGen.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

std::string currentNamespace = "";

std::string transformExpression(SilentOperand& expression)
{
    std::string output;
    #if DEBUG
        std::cout << "Transforming expression\n";
    #endif

    switch(expression.type)
    {
        case SilentOperandType::Assign:
            output += transformExpression(*expression.left);
            output += transformExpression(*expression.right);
            output += "assign\n";
        break;

        case SilentOperandType::Add:
            output += transformExpression(*expression.left);
            output += transformExpression(*expression.right);
            output += "+\n";
        break;

        case SilentOperandType::Subtract:
            output += transformExpression(*expression.left);
            output += transformExpression(*expression.right);
            output += "-\n";
        break;

        case SilentOperandType::Multiply:
            output += transformExpression(*expression.left);
            output += transformExpression(*expression.right);
            output += "*\n";
        break;

        case SilentOperandType::Divide:
            output += transformExpression(*expression.left);
            output += transformExpression(*expression.right);
            output += "/\n";
        break;

        case SilentOperandType::Number:
            output += expression.token->value+"\n";
        break;

        case SilentOperandType::Variable:
            output += expression.variable->name+"\n";
        break;
    }

    #if DEBUG
        std::cout << "Done transforming expression\n";
    #endif
    return output;
}

std::string transformVariable(SilentVariable& var)
{
    std::string output = "";
    #if DEBUG
        std::cout << "Transforming variable:" << var.name << "\n";
    #endif

    currentNamespace += "::"+var.name;
    output += "var "+currentNamespace+"\n";

    if(var.initialised) output += transformExpression(*var.expresion);

    for(uint64 i = 0; i < var.name.length()+2; i++) currentNamespace.pop_back();

    #if DEBUG
        std::cout << "Done transforming variable:" << var.name << "\n";
    #endif
    return output;
}

std::string transformLocalScope(SilentLocalScope& scope)
{
    std::string output = "";
    #if DEBUG
        std::cout << "Transforming local scope\n";
    #endif

    uint64 currentVar = 0;

    for(SilentStatement* statement : scope.statements)
    {
        switch(statement->type)
        {
            case SilentStatementType::VarInit:
                output += transformVariable(*scope.variables[currentVar]);
                currentVar++;
            break;
            default:
                std::cout << "default type parse\n";
            break;
        }
    }

    #if DEBUG
        std::cout << "Done transforming local scope\n";
    #endif
    return output;
}

std::string transformFunction(SilentFunction& function)
{
    std::string output = "";
    #if DEBUG
        std::cout << "Transforming function:" << function.name << "\n";
    #endif

    output += transformLocalScope(*function.scope);

    #if DEBUG
        std::cout << "Done transforming function:" << function.name << "\n";
    #endif
    return output;
}

std::string transformNamespace(SilentNamespace& scope)
{
    std::string output = "";
    #if DEBUG
        std::cout << "Transforming namespace:" << scope.name << "\n";
    #endif

    if(currentNamespace.length() == 0) currentNamespace = scope.name;

    for(SilentNamespace* scope : scope.namespaces) 
        output += transformNamespace(*scope);

    for(SilentFunction* function : scope.functions)
        output += transformFunction(*function);

    #if DEBUG
        std::cout << "Done transforming namespace:" << scope.name << "\n";
    #endif

    for(uint64 i = 0; i < scope.name.length(); i++) currentNamespace.pop_back();
    return output;
}

SilentIntCode* Silent::SilentTransform(SilentParserInfo* parsedCode)
{
    #if DEBUG
        std::cout << "Generating intermediate code...\n";
    #endif

    SilentIntCode* output = new SilentIntCode();
    output->code = "goto main\n";

    for(SilentNamespace* scope : parsedCode->namespaces) 
        output->code += transformNamespace(*scope);

    if(parsedCode->main != NULL)
    output->code += transformFunction(*parsedCode->main);

    #if DEBUG
        std::cout << "Done generating intermediate code...\n";
        std::cout << "Generated code:\n" << output->code;
    #endif
    return output;
}