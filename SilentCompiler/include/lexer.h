#pragma once
#include "common.h"

typedef enum TokenType
{
    // Primitives
    tkn_Void,
    tkn_I8,
    tkn_UI8,
    tkn_I16,
    tkn_UI16,
    tkn_I32,
    tkn_UI32,
    tkn_I64,
    tkn_UI64,
    tkn_F32,
    tkn_F64,
    tkn_String,

    // Keywords
    tkn_Subroutine,
    tkn_If,
    tkn_IfNot,
    tkn_Else,
    tkn_For,
    tkn_While,
    tkn_Type,
    tkn_Return,
    tkn_New,
    tkn_Delete,
    tkn_Namespace,
    tkn_Import,
    tkn_Inline,

    // Access modifiers
    tkn_Public,
    tkn_Private,
    tkn_Protected,

    // Export,
    tkn_Asm,

    // Arithmetic Symbols
    tkn_Add,
    tkn_Subtract,
    tkn_Multiply,
    tkn_Divide,
    
    // Comparison Operators
    tkn_Equal,
    tkn_NotEqual,
    tkn_LessThan,
    tkn_LessThanOrEqualTo,
    tkn_MoreThan,
    tkn_MoreThanOrEqualTo,
    
    // Logical Operators
    tkn_And,
    tkn_Or,
    tkn_Xor,
    tkn_Not,
    
    // Conditional Operator
    tkn_ConditionalAnd,
    tkn_ConditionalOr,
    
    // Bit Shift Operators
    tkn_RightShift,
    tkn_LeftShift,
    
    // Assignment Operators
    tkn_Assign,
    tkn_AddAssign,
    tkn_SubtractAssign,
    tkn_MultiplyAssign,
    tkn_DivideAssign,
    
    // Other Symbols
    tkn_Semicolon,
    tkn_Comma,
    tkn_Fullstop,
    tkn_OpenParam,
    tkn_CloseParam,
    tkn_OpenBracket,
    tkn_CloseBracket,
    tkn_OpenScope,
    tkn_CloseScope,
    tkn_Colon,
    
    // Other
    tkn_Identifier,
    tkn_Number,
    tkn_StringValue,
    tkn_Unrecognised,
    tkn_EndOfFile
}TokenType;

typedef struct Token{
    TokenType type;
    char* id;
}Token;

typedef struct TokenizerInfo{
    SymbolTableLink* symLink;

    Token* tokens;
    uint64 count;
    uint64 allocated;

    char* src;
    uint64 srcPtr;

    char success;

}TokenizerInfo;

TokenizerInfo Tokenize(char* source);










