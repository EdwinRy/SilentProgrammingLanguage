#include "SSemantics.hpp"

namespace Silent
{
namespace Semantics
{
    bool SemanticAnalyser::Analyse(
        SymbolTable* symbolTable, 
        Parser::Program* program)
    {
        analyser = this;
        return true;
    }

    bool Program::Analyse(Parser::Program* prog)
    {
        SymbolTable* symTable = prog->table;
        
        for (TableNode node : symTable->GetItems())
        {
            switch (node.nodeType)
            {
                case TableNode::Type::Namespace:
                {
                    Namespace scope;
                    if (scope.Analyse((Parser::Namespace*)node.GetNode()))
                        namespaces.push_back(scope);
                }
                break;

                case TableNode::Type::Subroutine:
                {

                }
                break;

                case TableNode::Type::Structure:
                {

                }
                break;


                default:
                break;
            }
        }


        return true;
    }

}}