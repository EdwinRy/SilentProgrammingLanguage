#pragma once
#include "SilentC.h"

typedef enum ParseNodeType{
    prs_NumSign,
    prs_Fullstop,
    prs_Numbers,
    prs_Id,
    
    
    prs_Integer,
    prs_Float,
    prs_Literal,
    prs_StringVal,
    prs_Identifier,
    prs_AssignmentLhs,

    prs_Factor,
    prs_Multiplication,
    prs_Addition,
    prs_Condition,
    prs_Assignment,
    prs_Expression,

    prs_Statement,
    prs_Program,

    prs_MulOp,
    prs_DivOp,
    prs_AddOp,
    prs_SubOp,
    prs_EqOp,
    prs_NEqOp,
    prs_LtOp,
    prs_MtOp,
    prs_LtEqOp,
    prs_MtEqOp,
    prs_AssOp
}ParseNodeType;

typedef struct ParseNode ParseNode;

typedef struct ParseNode{
    ParseNodeType type;
    ParseNode** children;
    uint64 childCount;
    uint64 childAlloc;
    Token token;
}ParseNode;

typedef struct ParseInfo {
    ParseNode* root;
    ParseNode* ptr;
    
    Token* tokens;
    uint64 tokenPtr;
    uint64 tokenCount;

}ParseInfo;

typedef struct SilentCompileInfo SilentCompileInfo;
char SilentParse(SilentCompileInfo* info);