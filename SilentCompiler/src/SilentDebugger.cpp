#include "SilentDebugger.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

//Outputting binary trees
std::string getOperandType(SilentOperandType type)
{
    switch(type)
    {
        case SilentOperandType::Add:
            return "+";
        break;
        case SilentOperandType::Subtract:
            return "-";
        break;
        case SilentOperandType::Multiply:
            return "*";
        break;
        case SilentOperandType::Divide:
            return "/";
        break;
        case SilentOperandType::Number:
            return "Num";
        break;
        case SilentOperandType::Variable:
            return "Var";
        break;
        case SilentOperandType::Assign:
            return "=";
        break;
        case SilentOperandType::Equal:
            return "==";
        break;
        case SilentOperandType::NotEqual:
            return "!=";
        break;
        case SilentOperandType::Larger:
            return ">";
        break;
        case SilentOperandType::LargerOrEqual:
            return ">=";
        break;
        case SilentOperandType::Smaller:
            return "<";
        break;
        case SilentOperandType::SmallerOrEqual:
            return "<=";
        break;
        case SilentOperandType::And:
            return "&";
        break;
        case SilentOperandType::Or:
            return "|";
        break;
        case SilentOperandType::Xor:
            return "^";
        break;
        case SilentOperandType::Not:
            return "!";
        break;
        default:
            return "Unknown";
        break;
    }
}

typedef struct Generation
{
    std::vector<SilentOperand*> operands;
}Generation;

std::vector<Generation> generations;
uint64 currentGen = 0;

void addGen();
void printGen()
{
    for(uint64 j = 0; j < generations[currentGen].operands.size(); j++)
    {
        //std::cout << "here\n";
        if(generations[currentGen].operands[j]->type == SilentOperandType::Number)
        {
            std::cout
            << generations[currentGen].operands[j]->token->value.data() 
            << " ";
        }
        else
        {
            std::cout
            << getOperandType(generations[currentGen].operands[j]->type).data() 
            << " ";
        }
    }
    std::cout << "\n";
    addGen();
}

void addGen()
{
    Generation gen;
    bool added = false;
    for(uint64 j = 0; j < generations[currentGen].operands.size(); j++)
    {
        if(generations[currentGen].operands[j]->type != SilentOperandType::Number &&
            generations[currentGen].operands[j]->type != SilentOperandType::Variable)
        {
            gen.operands.push_back(generations[currentGen].operands[j]->left);
            gen.operands.push_back(generations[currentGen].operands[j]->right);
            added = true;
        }
    }
    generations.push_back(gen);
    currentGen += 1;
    if(added)
    {
        printGen();
    }
}

void Silent::SilentPrintTree(SilentOperand* operand)
{
    Generation gen;
    gen.operands.push_back(operand);
    generations.push_back(gen);
    printGen();
}