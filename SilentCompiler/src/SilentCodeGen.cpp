#include "SilentCodeGen.hpp"
#include "SilentParser.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

namespace Silent
{
    void SilentCodeGenerator::Compile(SilentParser *parser)
    {
        cp = 0;
        code = "";
        CompileNamespace(*parser->GetGlobalNamespace());
    }

    std::string SilentCodeGenerator::GetOutput() { return code; }

    std::string SilentCodeGenerator::GenScopeName(std::string id)
    {
        std::string name = "";
        for(SilentNamespace *scope : namespaces)
        {
            name += scope->name;
            name += "::";
        }
        name += id;
        return name;
    }

    void SilentCodeGenerator::CompileExpression(
        SilentOperand &expression
    )
    {
        #if DEBUG
        std::cout << "Compiling expression\n";
        #endif
        switch(expression.type)
        {
            case SilentOperandType::Assign:
                currentType = 
                    expression.left->variable->type;
                printf("Assignment of type %i\n",currentType.primitive);
                
            break;

            case SilentOperandType::Number:
                if(currentType.isPrimitive)
                {
                    switch(currentType.primitive)
                    {
                        case SilentPrimitives::int8:
                        code += (char)SilentBytecode::Push1;
                        code += (char)SilentVMType::INT8;
                        code += 
                            (char)std::stoi(expression.token->value,nullptr,10);
                        break;

                        case SilentPrimitives::uint8:
                        code += (char)SilentBytecode::Push1;
                        code += (char)SilentVMType::UINT8;
                        code += '\0';
                        break;

                        case SilentPrimitives::int16:
                        code += (char)SilentBytecode::Push2;
                        code += (char)SilentVMType::INT16;
                        code += '\0';
                        code += '\0';
                        break;
                        case SilentPrimitives::uint16:
                        code += (char)SilentBytecode::Push2;
                        code += (char)SilentVMType::UINT16;
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::int32:
                        code += (char)SilentBytecode::Push4;
                        code += (char)SilentVMType::INT32;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::uint32:
                        code += (char)SilentBytecode::Push4;
                        code += (char)SilentVMType::UINT32;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::int64:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::INT64;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::uint64:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::UINT64;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::float32:
                        code += (char)SilentBytecode::Push4;
                        code += (char)SilentVMType::FLOAT32;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::float64:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::FLOAT64;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::string:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::POINTER;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::pointer:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::POINTER;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        default: break;
                    }
                }
            break;

            default: break;
        }
        #if DEBUG
        std::cout << "Done compiling expression\n";
        #endif
    }

    void SilentCodeGenerator::CompileStatement(SilentStatement &statement)
    {
        #if DEBUG
        std::cout << "Compiling statement\n";
        #endif

        switch(statement.type)
        {
            case SilentStatementType::VarInit:
                if(statement.variable->type.isPrimitive)
                {
                    switch(statement.variable->type.primitive)
                    {
                        case SilentPrimitives::int8:
                        code += (char)SilentBytecode::Push1;
                        code += (char)SilentVMType::INT8;
                        code += '\0';
                        break;

                        case SilentPrimitives::uint8:
                        code += (char)SilentBytecode::Push1;
                        code += (char)SilentVMType::UINT8;
                        code += '\0';
                        break;

                        case SilentPrimitives::int16:
                        code += (char)SilentBytecode::Push2;
                        code += (char)SilentVMType::INT16;
                        code += '\0';
                        code += '\0';
                        break;
                        case SilentPrimitives::uint16:
                        code += (char)SilentBytecode::Push2;
                        code += (char)SilentVMType::UINT16;
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::int32:
                        code += (char)SilentBytecode::Push4;
                        code += (char)SilentVMType::INT32;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::uint32:
                        code += (char)SilentBytecode::Push4;
                        code += (char)SilentVMType::UINT32;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::int64:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::INT64;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::uint64:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::UINT64;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::float32:
                        code += (char)SilentBytecode::Push4;
                        code += (char)SilentVMType::FLOAT32;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::float64:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::FLOAT64;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::string:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::POINTER;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        case SilentPrimitives::pointer:
                        code += (char)SilentBytecode::Push8;
                        code += (char)SilentVMType::POINTER;
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        code += '\0';
                        break;

                        default: break;
                    }
                }
                else
                {

                }
            break;

            case SilentStatementType::Expression:
                CompileExpression(*statement.expression);
            break;

            default: break;
        }
        
        #if DEBUG
        std::cout << "Done compiling statement\n";
        #endif
    }

    void SilentCodeGenerator::CompileLocalScope(SilentLocalScope &scope)
    {
        #if DEBUG
        std::cout << "Compiling local scope\n";
        #endif
    
        for(SilentStatement* statement : scope.statements)
        {
            CompileStatement(*statement);
        }

        #if DEBUG
        std::cout << "Done compiling local scope\n";
        #endif
    }

    void SilentCodeGenerator::CompileFunction(SilentFunction &func)
    {
        #if DEBUG
        std::cout << "Compiling function: " << func.name << "\n";
        #endif

        std::cout << "Scope name: " << GenScopeName(func.name) << "\n";
        CompileLocalScope(*func.scope);

        #if DEBUG
        std::cout << "Done compiling function: " << func.name << "\n";
        #endif
    }
    void SilentCodeGenerator::CompileNamespace(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Compiling namespace: " << scope.name << "\n";
        #endif

        namespaces.push_back(&scope);

        //Compile nested namespace
        for(SilentNamespace* nScope : scope.namespaces)
            CompileNamespace(*nScope);

        //Compile namespace functions
        for(SilentFunction* func : scope.functions)
            CompileFunction(*func);

        namespaces.pop_back();

        #if DEBUG
        std::cout << "Done compiling namespace: " << scope.name << "\n";
        #endif
    }
}