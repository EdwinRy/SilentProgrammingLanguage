#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

#define ChildInit 16

#define AppendTokenNode(PARSER, PARENT, NODETYPE, TOKEN) \
        {ParseNode* tempNode_append = ParseNodeInit(NODETYPE); \
        tempNode_append->token = TOKEN; \
        NextToken(PARSER); \
        ParseNodeAppend(PARENT, tempNode_append); \
        TOKEN = GetToken(PARSER);}

#define FreeProdFail(PTR, PTRSAVED, PROD) \
        PTR = PTRSAVED; \
        ParseNodeFree(PROD); \
        return 0;

#define IfProdSuccessAppend(PROD, NODE) \
        if(PROD(parser) == 1) ParseNodeAppend(NODE, parser->ptr);

#define ParseNodeTrimRet(NODE) \
        ParseNodeTrim(NODE); \
        parser->ptr = NODE; \
        return 1; 

#define ParseNodeFailRet(NODE, REVVAR) \
        parser->tokenPtr = REVVAR; \
        ParseNodeFree(NODE); \
        return 0;

#define Expect(PROD, NODE, REVVAR) \
        if(PROD(parser) == 0) {ParseNodeFailRet(NODE, REVVAR);}

#define ExpectAppend(PROD, NODE, REVVAR) \
        Expect(PROD, NODE, REVVAR) \
        else {ParseNodeAppend(NODE, parser->ptr);}

#define ParseNodeToSingleChild(NODE) \
        if(NODE->childCount == 1) { \
            ParseNode* _temp = NODE; \
            NODE = NODE->children[0]; \
            free(_temp->children); \
            free(_temp); \
        } \


static ParseNode* ParseNodeInit(ParseNodeType type)
{
    ParseNode* node = calloc(1, sizeof(ParseNode));
    node->type = type;
    node->childCount = 0;
    switch(type)
    {
        case prs_Program:
            node->children = calloc(ChildInit, sizeof(ParseNode*));
            node->childAlloc = ChildInit;
        break;

        case prs_Literal:
        case prs_Expression:
            node->children = calloc(1, sizeof(ParseNode*));
            node->childAlloc = 1;
        break;

        case prs_Integer:
            node->children = calloc(2, sizeof(ParseNode*));
            node->childAlloc = 2;
        break;

        case prs_Multiplication:
        case prs_Addition:
        case prs_Condition:
        case prs_Assignment:
            node->children = calloc(3, sizeof(ParseNode*));
            node->childAlloc = 3;
        break;

        case prs_Float:
            node->children = calloc(4, sizeof(ParseNode*));
            node->childAlloc = 4;
        break;

    }
    return node;
}

static void ParseNodeFree(ParseNode* node)
{
    if(node)
    {
        if(node->children)
        {
            for(int i = 0; i < node->childCount; i++)
                ParseNodeFree(node->children[i]);

            free(node->children);
        }
        free(node);
    }
}


static char ParseNodeAppend(ParseNode* parent, ParseNode* child)
{
    if(parent->childCount == parent->childAlloc)
    {
        ParseNode** temp = realloc(parent->children, 
            (parent->childAlloc + ChildInit) * sizeof(ParseNode*));

        if(temp == NULL) return 0;
        parent->children = temp;
        parent->childAlloc += ChildInit;
    }

    parent->children[parent->childCount++] = child;
    return 1;
}

static Token GetToken(ParseInfo* parser)
{
    if(parser->tokenCount <= parser->tokenPtr)
    {
        Token tkn = {0};
        tkn.type = tkn_EndOfFile;
        return tkn;
    }
    return parser->tokens[parser->tokenPtr];
}

static Token Peak(ParseInfo* parser)
{
    if(parser->tokenCount <= parser->tokenPtr + 1)
    {
        Token tkn = {0};
        tkn.type = tkn_EndOfFile;
        return tkn;
    }
    return parser->tokens[parser->tokenPtr + 1];
}

static Token PeakAhead(ParseInfo* parser, int i)
{
    if(parser->tokenCount <= parser->tokenPtr + i)
    {
        Token tkn = {0};
        tkn.type = tkn_EndOfFile;
        return tkn;
    }
    return parser->tokens[parser->tokenPtr + i];
}

static void NextToken(ParseInfo* parser)
{
    parser->tokenPtr++;
}

static void ParseNodeTrim(ParseNode* node)
{
    if(node->childAlloc != node->childCount)
    {
        node->childAlloc = node->childCount;
        ParseNode** temp = realloc(node->children, 
            node->childAlloc * sizeof(ParseNode*));
        if(temp != NULL) node->children = temp;
    }
}

static char Integer(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* integer = ParseNodeInit(prs_Integer);

    if(tkn.type == tkn_Add || tkn.type == tkn_Subtract)
    {
        if(Peak(parser).type != tkn_Number)
        {
            ParseNodeFree(integer);
            return 0;
        }

        AppendTokenNode(parser, integer, prs_NumSign, tkn);
    }

    if(tkn.type == tkn_Number)
    {
        AppendTokenNode(parser, integer, prs_Numbers, tkn);
        ParseNodeTrimRet(integer);
    }
    else ParseNodeFailRet(integer, tknPtr);
}

static char Float(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* flt = ParseNodeInit(prs_Float);

    if(tkn.type == tkn_Add || tkn.type == tkn_Subtract)
    {
        Token peak = Peak(parser);
        if(peak.type != tkn_Number || peak.type != tkn_Fullstop)
        {
            ParseNodeFree(flt);
            return 0;
        }
        AppendTokenNode(parser, flt, prs_NumSign, tkn);
    }

    if(tkn.type == tkn_Number && Peak(parser).type == tkn_Fullstop
        && PeakAhead(parser, 2).type == tkn_Number)
    {
        AppendTokenNode(parser, flt, prs_NumSign, tkn);
        AppendTokenNode(parser, flt, prs_Fullstop, tkn);
        AppendTokenNode(parser, flt, prs_Numbers, tkn);
    }
    else if(tkn.type == tkn_Fullstop && Peak(parser).type == tkn_Number)
    {
        AppendTokenNode(parser, flt, prs_Fullstop, tkn);
        AppendTokenNode(parser, flt, prs_Numbers, tkn);
    }
    else {FreeProdFail(parser->tokenPtr, tknPtr, flt);}
    ParseNodeTrimRet(flt);
}

static char Literal(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* lit = ParseNodeInit(prs_Literal);

    if(tkn.type == tkn_StringValue)
    {
        AppendTokenNode(parser, lit, prs_StringVal, tkn);
    }
    else IfProdSuccessAppend(Float, lit)
    else IfProdSuccessAppend(Integer, lit)
    else { FreeProdFail(parser->tokenPtr, tknPtr, lit); }

    parser->ptr = lit; 
    return 1; 
}

static char Identifier(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* id = ParseNodeInit(prs_Identifier);

    if(tkn.type == tkn_Identifier)
    {
        AppendTokenNode(parser, id, prs_Id , tkn);
        ParseNodeTrimRet(id);
    }
    else
    {
        ParseNodeFree(id);
        return 0;
    }
}

static char Expression(ParseInfo* parser);

static char Factor(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* fac = ParseNodeInit(prs_Factor);

    if(tkn.type == tkn_OpenParam)
    {
        ExpectAppend(Expression, fac, tknPtr);
        tkn = GetToken(parser);
        if(tkn.type != tkn_CloseParam) ParseNodeFailRet(fac, tknPtr);
    }
    else IfProdSuccessAppend(Literal, fac)
    else {FreeProdFail(parser->tokenPtr, tknPtr, fac);}
    ParseNodeTrimRet(fac);
}

static char Multiplication(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* mul = ParseNodeInit(prs_Multiplication);

    if(Factor(parser) == 1)
    {
        ParseNodeAppend(mul, parser->ptr);
    }
    else
    {
        ParseNodeFree(mul);
        return 0;
    }

    for(;;)
    {
        tkn = GetToken(parser);
        if(tkn.type == tkn_Multiply)
        {
            AppendTokenNode(parser, mul, prs_MulOp , tkn);
            ExpectAppend(Factor, mul, tknPtr);
        }
        else if(tkn.type == tkn_Divide)
        {
            AppendTokenNode(parser, mul, prs_DivOp , tkn);
            ExpectAppend(Factor, mul, tknPtr);
        }
        else break;
    }

    // if(mul->childCount == 1){
    //     ParseNode* temp = mul;
    //     mul = mul->children[0];
    //     free(temp->children);
    //     free(temp);
    // }
    ParseNodeToSingleChild(mul);
    ParseNodeTrimRet(mul);
}

static char Addition(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* add = ParseNodeInit(prs_Addition);

    if(Multiplication(parser) == 1)
    {
        ParseNodeAppend(add, parser->ptr);
    }
    else
    {
        ParseNodeFree(add);
        return 0;
    }

    for(;;)
    {
        tkn = GetToken(parser);
        if(tkn.type == tkn_Add)
        {
            AppendTokenNode(parser, add, prs_AddOp , tkn);
            ExpectAppend(Multiplication, add, tknPtr);
        }
        else if(tkn.type == tkn_Subtract)
        {
            AppendTokenNode(parser, add, prs_SubOp , tkn);
            ExpectAppend(Multiplication, add, tknPtr);
        }
        else break;
    }

    ParseNodeToSingleChild(add);
    ParseNodeTrimRet(add);
}

static char Condition(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* con = ParseNodeInit(prs_Condition);

    if(Addition(parser) == 1)
    {
        ParseNodeAppend(con, parser->ptr);
    }
    else
    {
        ParseNodeFree(con);
        return 0;
    }

    for(;;)
    {
        tkn = GetToken(parser);
        switch(tkn.type)
        {
            
            case tkn_Equal:
                AppendTokenNode(parser, con, prs_EqOp , tkn);
                ExpectAppend(Addition, con, tknPtr);
            break;

            case tkn_NotEqual:
                AppendTokenNode(parser, con, prs_NEqOp , tkn);
                ExpectAppend(Addition, con, tknPtr);
            break;

            case tkn_LessThan:
                AppendTokenNode(parser, con, prs_LtOp , tkn);
                ExpectAppend(Addition, con, tknPtr);
            break;

            case tkn_MoreThan:
                AppendTokenNode(parser, con, prs_MtOp , tkn);
                ExpectAppend(Addition, con, tknPtr);
            break;

            case tkn_LessThanOrEqualTo:
                AppendTokenNode(parser, con, prs_LtEqOp , tkn);
                ExpectAppend(Addition, con, tknPtr);
            break;

            case tkn_MoreThanOrEqualTo:
                AppendTokenNode(parser, con, prs_MtEqOp , tkn);
                ExpectAppend(Addition, con, tknPtr);
            break;

            default:
                ParseNodeToSingleChild(con);
                ParseNodeTrimRet(con);
            break;
        }
    }
}

static char AssignmentLhs(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* lhs = ParseNodeInit(prs_AssignmentLhs);

    if(Identifier(parser) == 1)
    {
        ParseNodeAppend(lhs, parser->ptr);
        ParseNodeTrimRet(lhs);
    }
    else
    {
        ParseNodeFree(lhs);
        return 0;
    }
}

static char Assignment(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* ass = ParseNodeInit(prs_Assignment);

    if(AssignmentLhs(parser) == 1)
    {
        ParseNodeAppend(ass, parser->ptr);
    }
    else
    {
        ParseNodeFree(ass);
        return 0;
    }

    for(;;)
    {
        tkn = GetToken(parser);
        if(tkn.type == tkn_Assign)
        {
            AppendTokenNode(parser, ass, prs_AssOp , tkn);
            ExpectAppend(Condition, ass, tknPtr);
        }
        else break;
    }

    ParseNodeTrimRet(ass);
}

static char Expression(ParseInfo* parser)
{
    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    ParseNode* exp = ParseNodeInit(prs_Expression);
    
    ExpectAppend(Assignment, exp, tknPtr);

    tkn = GetToken(parser);
    if(tkn.type != tkn_Semicolon) 
    {
        ParseNodeFailRet(exp, tknPtr);
    }
    NextToken(parser);

    ParseNodeTrimRet(exp);
}

static char Program(ParseInfo* parser)
{
    ParseNode* node = ParseNodeInit(prs_Program);
    parser->root = node;
    parser->ptr = node;

    uint64 tknPtr = parser->tokenPtr;
    Token tkn = GetToken(parser);

    while(Expression(parser) == 1)
    {
        ParseNodeAppend(node, parser->ptr);
    }

    // ExpectAppend(Expression, node, tknPtr);
    ParseNodeTrimRet(node);
}

static ParseInfo* ParseInfoInit(SilentCompileInfo* info)
{
    ParseInfo* parser = calloc(1, sizeof(ParseInfo));
    parser->tokens = info->tokenizer.tokens;
    parser->tokenCount = info->tokenizer.count;
    return parser;
}

char SilentParse(SilentCompileInfo* info)
{
    ParseInfo* parser = ParseInfoInit(info);
    info->parser = parser;

    if(Program(parser)) return 1;
    else return 0;
}



void ParseInfoFree(ParseInfo* info)
{

}



