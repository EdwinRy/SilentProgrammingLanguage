#include "silent_parser.h"
#include <stdlib.h>
#include <stdio.h>

Parser ParserInit(Tokenizer tokenizer)
{
    Parser parser = { 0 };
    parser.tokenizer = tokenizer;
    parser.root.type = PARSER_TYPE_GLOBAL;
    parser.root.global = calloc(1, sizeof(ParserGlobal));
    parser.root.global->node = &parser.root;
    parser.root.parent = NULL;
    return parser;
}

void Parse(Parser parser)
{

}

void ParserTest(Parser parser)
{
    Tokenizer* tokenizer = &parser.tokenizer;
    
    while(!IsTokenType(tokenizer, tkn_EndOfFile))
    {
        switch(GetTokenType(tokenizer))
        {
            case tkn_Namespace:
                if(!ParseNamespace(&parser))
                    printf("Failed to parse namespace\n");
            break;

            default:
                NextToken(tokenizer);
            break;
        }
    }
}

inline void EnterNode(Parser* parser, ParserNodeType type, void* obj)
{
    ParserNode* node = calloc(1, sizeof(ParserNode));
    node->type = type;
    node->parent = parser->current;
    node->nodeObj = obj;
    parser->current = node;
}

inline void ExitNode(Parser* parser)
{
    parser->current = parser->current->parent;
}

char ParseNamespace(Parser* parser)
{
    Tokenizer* tokenizer = &parser->tokenizer;
    if(!ExpectToken(tokenizer, tkn_Namespace)) return 0;
    if(GetTokenType(tokenizer) != tkn_Identifier) return 0;

    ParserNamespace* namespace = calloc(1, sizeof(ParserNamespace));
    namespace->id = GetToken(tokenizer).val;
    EnterNode(parser, PARSER_TYPE_NAMESPACE, namespace);
    namespace->node = parser->current;

    if(!ParseNamespaceScope(parser))
    {
        
    }

    ExitNode(parser);
    return 1;
}

char ParseNamespaceScope(Parser* parser)
{

}

ParserNode ParseFunction()
{

}

ParserNode ParseWhileLoop()
{

}

ParserNode ParseForLoop()
{

}

ParserNode ParseType()
{

}

ParserNode ParseMethod()
{

}

ParserNode ParseStatement()
{

}

ParserNode ParseCall()
{

}

ParserNode ParseExpression()
{

}

ParserNode ParseMultiplication()
{

}

ParserNode ParseAddition()
{

}

ParserNode ParseCondition()
{

}

ParserNode ParseLogicalOp()
{

}

char ParseFactor(Parser parser)
{
    
    return 0;
}