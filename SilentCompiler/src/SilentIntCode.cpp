//#include "SilentCodeGen.hpp"
#include "SilentIntCode.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

namespace Silent
{
    bool SilentIntCode::SilentTransform(SilentParser *parser)
    {
        #if DEBUG
        std::cout << "Generating intermediate code...\n";
        #endif

        std::string output = "goto main\n";
        code = "goto main\n";

        TransformNamespace(*parser->GetGlobalNamespace());

        //if(parsedCode->main != NULL)
        //code += transformFunction(*parsedCode->main);

        #if DEBUG
        std::cout << "Done generating intermediate code...\n";
        std::cout << "Generated code:\n" << code;
        #endif
        return true;
    }

    std::string SilentIntCode::TypeToString(SilentPrimitives dt)
    {
        switch(dt)
        {
            case SilentPrimitives::int8: return "int8"; break;
            case SilentPrimitives::uint8: return "uint8"; break;
            case SilentPrimitives::int16: return "int16"; break;
            case SilentPrimitives::uint16: return "uint16"; break;
            case SilentPrimitives::int32: return "int32"; break;
            case SilentPrimitives::uint32: return "uint32"; break;
            case SilentPrimitives::int64: return "int64"; break;
            case SilentPrimitives::uint64: return "uint64"; break;
            case SilentPrimitives::float32: return "float32"; break;
            case SilentPrimitives::float64: return "float64"; break;
            case SilentPrimitives::string: return "string"; break;
            case SilentPrimitives::pointer: return "pointer"; break;
            default : return "null"; break;
        }
    }

    void SilentIntCode::TransformExpression(SilentOperand &expression)
    {
        #if DEBUG
        std::cout << "Transforming expression\n";
        #endif

        switch(expression.type)
        {
            case SilentOperandType::Assign:
                TransformExpression(*expression.left);
                TransformExpression(*expression.right);
                code += "=\n";
            break;

            case SilentOperandType::Add:
                TransformExpression(*expression.left);
                TransformExpression(*expression.right);
                code += "+\n";
            break;

            case SilentOperandType::Subtract:
                TransformExpression(*expression.left);
                TransformExpression(*expression.right);
                code += "-\n";
            break;

            case SilentOperandType::Multiply:
                TransformExpression(*expression.left);
                TransformExpression(*expression.right);
                code += "*\n";
            break;

            case SilentOperandType::Divide:
                TransformExpression(*expression.left);
                TransformExpression(*expression.right);
                code += "/\n";
            break;

            case SilentOperandType::Number:
                code += "p " + expression.token->value+"\n";
            break;

            case SilentOperandType::Variable:
                code += "l " + expression.variable->name+"\n";
            break;

            default:
            break;
        }

        #if DEBUG
            std::cout << "Done transforming expression\n";
        #endif
    }

    void SilentIntCode::TransformVariable(SilentVariable &var)
    {
        #if DEBUG
        std::cout << "Transforming variable:" << var.name << "\n";
        #endif

        code += "v " + var.name + " " + std::to_string(var.size) + "\n";

        //if(var.initialised) TransformExpression(*var.expression);

        #if DEBUG
        std::cout << "Done transforming variable:" << var.name << "\n";
        #endif
    }

    void SilentIntCode::TransformLocalScope(SilentLocalScope &scope)
    {
        #if DEBUG
        std::cout << "Transforming local scope\n";
        #endif

        uint64 currentVar = 0;

        for(SilentStatement* statement : scope.statements)
        {
            switch(statement->type)
            {
                case SilentStatementType::VarInit:
                    TransformVariable(*scope.variables[currentVar]);
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
    }

    void SilentIntCode::TransformFunction(SilentFunction &function)
    {
        #if DEBUG
        std::cout << "Transforming function:" << function.name << "\n";
        #endif

        code += "f " + function.name + "\n";

        code += "a\n";
        TransformLocalScope(*function.parameters);
        code += "e a\n";
        TransformLocalScope(*function.scope);
        code += "e f " + function.name + "\n";

        #if DEBUG
        std::cout << "Done transforming function:" << function.name << "\n";
        #endif
    }

    void SilentIntCode::TransformStructure(SilentStructure &structure)
    {
        #if DEBUG
        std::cout << "Transforming structure:" << structure.name << "\n";
        #endif

        code += "s " + std::to_string(structure.size) + " " + structure.name + "\n";
        TransformLocalScope(*structure.variables);
        code += "e s " + structure.name + "\n";

        #if DEBUG
        std::cout << "Done transforming structure:" << structure.name << "\n";
        #endif
    }

    void SilentIntCode::TransformNamespace(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Transforming namespace:" << scope.name << "\n";
        #endif

        code += "n " + scope.name + "\n";

        for(SilentNamespace* scope : scope.namespaces) 
            TransformNamespace(*scope);

        for(SilentStructure* structure : scope.types) 
            TransformStructure(*structure);

        for(SilentFunction* function : scope.functions)
            TransformFunction(*function);

        code += "e n " + scope.name + "\n";

        #if DEBUG
        std::cout << "Done transforming namespace:" << scope.name << "\n";
        #endif
    }

    std::vector<std::string> SilentIntCode::SplitString(
        std::string str, char splitChar)
    {
        std::vector<std::string> output;
        std::string buffer = "";

        for(char& c : str)
        {
            if(c == splitChar)
            {
                output.push_back(buffer);
                buffer.clear();
            }
            buffer += c;
        }
        return output;
    }
}