#include <iostream>
#include "SParser.hpp"
#pragma once

namespace Silent
{
    /*static class SymTablePrinter
    {
        public:
        static void Out(std::string str, uint64_t currentTab)
        {
            std::string tabs = "";
            for (uint64_t i = 0; i < currentTab; i++) tabs += "\t";
            std::cout << tabs << str << std::endl;
        }

        SymTablePrinter()
        {
        }

        static void PrintSymTable(SymbolTable* symTable)
        {
            PrintNode(symTable->self, 0);
        }

        static void PrintNode(TableNode node, uint64_t currentTab)
        {
            switch (node.nodeType)
            {
                case TableNode::Type::Program:
                {
                    Program* p = (Program*)node.GetNode();
                    Out("Program", currentTab);
                    currentTab++;
                    for(TableNode node : p->table->GetItems())
                        PrintNode(node, currentTab);
                    currentTab--;
                }
                break;

                case TableNode::Type::Namespace:
                {
                    Namespace* n = (Namespace*)node.GetNode();
                    Out("Namespace " + n->GetId(), currentTab);
                    currentTab++;
                    for (TableNode node : n->GetTable()->GetItems())
                        PrintNode(node, currentTab);
                    currentTab--;
                }
                break;

                case TableNode::Type::Subroutine:
                {
                    Subroutine* s = (Subroutine*)node.GetNode();
                    Out("Subroutine " + s->GetId(), currentTab);
                    currentTab++;
                    for (TableNode node : s->GetTable()->GetItems())
                        PrintNode(node, currentTab);
                    currentTab--;
                }
                break;

                case TableNode::Type::Variable:
                {
                    Variable* v = (Variable*)node.GetNode();
                    Out("Variable " + v->GetId(), currentTab);
                }
                break;

                case TableNode::Type::Structure:
                {
                    Type* t = (Type*)node.GetNode();
                    Out("Type " + t->GetId(), currentTab);
                    currentTab++;
                    for (TableNode node : t->GetTable()->GetItems())
                        PrintNode(node, currentTab);
                    currentTab--;
                }
                break;
            }
        }


    };*/
}