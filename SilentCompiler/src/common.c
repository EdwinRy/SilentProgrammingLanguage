#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SilentC.h"
#include "common.h"

char* GetPrsStr(ParseNodeType type);
char* GetTknStr(TokenType type);

static void PrintAst(ParseNode* node, int degree)
{
    char* space = calloc((degree * 4) + 1, sizeof(char));
    memset(space, ' ', (degree * 4));
    space[(degree * 4) + 1] = '\0';
    printf("%s%s : %lli", space, GetPrsStr(node->type), node->childCount);
    if(node->token.id != NULL) printf(" : \"%s\"", node->token.id);
    printf("\n");
    free(space);
    degree++;
    for(int i = 0; i < node->childCount; i++)
    {
        PrintAst(node->children[i], degree);
    }
}

void PrintParserAst(ParseNode* node)
{
    PrintAst(node, 0);
    printf("\n");
}

void PrintTokens(Token* tokens, uint64 count)
{
    for(int i = 0; i < count; i++)
    {
        printf("%s : %s\n", GetTknStr(tokens[i].type), 
            tokens[i].id == 0 ? "" : tokens[i].id);
    }
    printf("\n");
}

char* GetPrsStr(ParseNodeType type)
{
    switch(type)
    {
        case prs_NumSign: return "prs_NumSign";
        case prs_Fullstop: return "prs_Fullstop";
        case prs_Numbers: return "prs_Numbers";
        case prs_Id: return "prs_Id";
        case prs_Integer: return "prs_Integer";
        case prs_Float: return "prs_Float";
        case prs_Literal: return "prs_Literal";
        case prs_StringVal: return "prs_StringVal";
        case prs_Identifier: return "prs_Identifier";
        case prs_AssignmentLhs: return "prs_AssignmentLhs";
        case prs_Factor: return "prs_Factor";
        case prs_Multiplication: return "prs_Multiplication";
        case prs_Addition: return "prs_Addition";
        case prs_Condition: return "prs_Condition";
        case prs_Assignment: return "prs_Assignment";
        case prs_Expression: return "prs_Expression";
        case prs_Statement: return "prs_Statement";
        case prs_Program: return "prs_Program";
        case prs_MulOp: return "prs_MulOp";
        case prs_DivOp: return "prs_DivOp";
        case prs_AddOp: return "prs_AddOp";
        case prs_SubOp: return "prs_SubOp";
        case prs_EqOp: return "prs_EqOp";
        case prs_NEqOp: return "prs_NEqOp";
        case prs_LtOp: return "prs_LtOp";
        case prs_MtOp: return "prs_MtOp";
        case prs_LtEqOp: return "prs_LtEqOp";
        case prs_MtEqOp: return "prs_MtEqOp";
        case prs_AssOp: return "prs_AssOp";
        default: return "undef";
    }
}

char* GetTknStr(TokenType type)
{
    switch(type)
    {
        case tkn_Void: return "tkn_Void";
        case tkn_I8: return "tkn_I8";
        case tkn_UI8: return "tkn_UI8";
        case tkn_I16: return "tkn_I16";
        case tkn_UI16: return "tkn_UI16";
        case tkn_I32: return "tkn_I32";
        case tkn_UI32: return "tkn_UI32";
        case tkn_I64: return "tkn_I64";
        case tkn_UI64: return "tkn_UI64";
        case tkn_F32: return "tkn_F32";
        case tkn_F64: return "tkn_F64";
        case tkn_String: return "tkn_String";
        case tkn_Subroutine: return "tkn_Subroutine";
        case tkn_If: return "tkn_If";
        case tkn_IfNot: return "tkn_IfNot";
        case tkn_Else: return "tkn_Else";
        case tkn_For: return "tkn_For";
        case tkn_While: return "tkn_While";
        case tkn_Type: return "tkn_Type";
        case tkn_Return: return "tkn_Return";
        case tkn_New: return "tkn_New";
        case tkn_Delete: return "tkn_Delete";
        case tkn_Namespace: return "tkn_Namespace";
        case tkn_Import: return "tkn_Import";
        case tkn_Inline: return "tkn_Inline";
        case tkn_Public: return "tkn_Public";
        case tkn_Private: return "tkn_Private";
        case tkn_Protected: return "tkn_Protected";
        case tkn_Asm: return "tkn_Asm";
        case tkn_Add: return "tkn_Add";
        case tkn_Subtract: return "tkn_Subtract";
        case tkn_Multiply: return "tkn_Multiply";
        case tkn_Divide: return "tkn_Divide";
        case tkn_Equal: return "tkn_Equal";
        case tkn_NotEqual: return "tkn_NotEqual";
        case tkn_LessThan: return "tkn_LessThan";
        case tkn_LessThanOrEqualTo: return "tkn_LessThanOrEqualTo";
        case tkn_MoreThan: return "tkn_MoreThan";
        case tkn_MoreThanOrEqualTo: return "tkn_MoreThanOrEqualTo";
        case tkn_And: return "tkn_And";
        case tkn_Or: return "tkn_Or";
        case tkn_Xor: return "tkn_Xor";
        case tkn_Not: return "tkn_Not";
        case tkn_ConditionalAnd: return "tkn_ConditionalAnd";
        case tkn_ConditionalOr: return "tkn_ConditionalOr";
        case tkn_RightShift: return "tkn_RightShift";
        case tkn_LeftShift: return "tkn_LeftShift";
        case tkn_Assign: return "tkn_Assign";
        case tkn_AddAssign: return "tkn_AddAssign";
        case tkn_SubtractAssign: return "tkn_SubtractAssign";
        case tkn_MultiplyAssign: return "tkn_MultiplyAssign";
        case tkn_DivideAssign: return "tkn_DivideAssign";
        case tkn_Semicolon: return "tkn_Semicolon";
        case tkn_Comma: return "tkn_Comma";
        case tkn_Fullstop: return "tkn_Fullstop";
        case tkn_OpenParam: return "tkn_OpenParam";
        case tkn_CloseParam: return "tkn_CloseParam";
        case tkn_OpenBracket: return "tkn_OpenBracket";
        case tkn_CloseBracket: return "tkn_CloseBracket";
        case tkn_OpenScope: return "tkn_OpenScope";
        case tkn_CloseScope: return "tkn_CloseScope";
        case tkn_Colon: return "tkn_Colon";
        case tkn_Identifier: return "tkn_Identifier";
        case tkn_Number: return "tkn_Number";
        case tkn_StringValue: return "tkn_StringValue";
        case tkn_Unrecognised: return "tkn_Unrecognised";
        case tkn_EndOfFile: return "tkn_EndOfFile";    
    }
}