#include "silent_lexer.h"
#pragma once

typedef enum ParserNodeType {
    PARSER_TYPE_NAMESPACE,
    PARSER_TYPE_TYPE,
    PARSER_TYPE_FUNCTION,
    PARSER_TYPE_METHOD,
    PARSER_TYPE_EXPRESSION,
    PARSER_TYPE_IF_STATEMENT,
    PARSER_TYPE_FOR_LOOP,
    PARSER_TYPE_WHILE_LOOP,
    PARSER_TYPE_FUNC_CALL,
    PARSER_TYPE_VAR_DEC,
    PARSER_TYPE_GLOBAL,
    PARSER_TYPE_STATEMENT

}ParserNodeType;

typedef enum ParserFactorType
{
    PARSER_FACTOR_NUM,
    PARSER_FACTOR_ID,
    PARSER_FACTOR_EXPRESSION
}ParserFactorType;

typedef struct ParserNode ParserNode;

typedef struct ParserNamespace
{
    ParserNode* node;
    char* id;
    ParserNode* members;
    int members_c;

}ParserNamespace;

typedef struct ParserType {
    ParserNode* node;
    char* id;
}ParserType;

typedef struct ParserFunction {
    ParserNode* node;
    char* id;
}ParserFunction;

typedef struct ParserMethod {
    ParserNode* node;
    char* id;
}ParserMethod;

typedef struct ParserExpression {
    ParserNode* node;
    char* id;
}ParserExpression;

typedef struct ParserIfStatement {
    ParserNode* node;

}ParserIfStatement;

typedef struct ParserForLoop {
    ParserNode* node;
    char* id;
}ParserForLoop;

typedef struct ParserWhileLoop {
    ParserNode* node;
}ParserWhileLoop;

typedef struct ParserGlobal {
    ParserNode* node;
    ParserNode* members;
    int member_c;
}ParserGlobal;

typedef struct ParserFactor {
    ParserNode* node;
    ParserFactorType type;

}ParserFactor;

typedef struct ParserNode
{
    ParserNode* parent;
    ParserNodeType type;

    union 
    {
        ParserNamespace* parserNamespace;
        ParserGlobal* global;
        void* nodeObj;
    };

}ParserNode;

typedef struct Parser
{
    Tokenizer tokenizer;
    ParserNode root;
    ParserNode* current;
}Parser;

Parser ParserInit(Tokenizer tokenizer);
void Parse(Parser parser);
void ParserTest(Parser parser);
char ParseNamespace(Parser* parser);


