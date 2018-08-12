#include "SilentCodeGen.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef std::vector<Silent::SilentToken> TokenList;
typedef std::vector<Silent::SilentToken*> TokenPtrList;
//typedef std::vector<SilentNode> NodeList;
typedef std::vector<SilentNode*> NodePtrList;


std::vector<SilentIntCode>* SilentTransform(std::vector<SilentNode*>& nodes)
{
    std::vector<SilentIntCode>* out = new std::vector<SilentIntCode>();
    for(uint64 i = 0; i < nodes.size(); i++)
    {
        switch(nodes[i]->type)
        {
            case SilentNodeType::expression:
            break;

            case SilentNodeType::function:
            break;

            case SilentNodeType::scope:
            break;

            case SilentNodeType::structure:
            break;

            case SilentNodeType::variable:
            break;
        }
    }
    return out;
}

//Outputting binary trees
std::string getOperandType(SilentOperandType type)
{
    switch(type)
    {
        case SilentOperandType::Add:
            return "Add";
        break;
        case SilentOperandType::Subtract:
            return "Subtract";
        break;
        case SilentOperandType::Multiply:
            return "Multiply";
        break;
        case SilentOperandType::Divide:
            return "Divide";
        break;
        case SilentOperandType::Identifier:
            return "id";
        break;
    }
}

typedef struct Generation
{
    std::vector<SilentOperand*> operands;
}Generation;

std::vector<Generation> generations;
uint64 currentGen;

void addGen();

void printGen()
{
    for(uint64 j = 0; j < generations[currentGen].operands.size(); j++)
    {
        if(generations[currentGen].operands[j]->type == SilentOperandType::Value)
        {
            std::cout 
            << generations[currentGen].operands[j]->tokenData->value.data() 
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
        if(generations[currentGen].operands[j]->type != SilentOperandType::Value &&
            generations[currentGen].operands[j]->type != SilentOperandType::Identifier)
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