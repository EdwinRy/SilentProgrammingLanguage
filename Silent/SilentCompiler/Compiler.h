#include "Lexer.h"
#include <stdio.h>
#pragma once

typedef struct Program 
{
	char* source;
	unsigned long sourceLength;
	char* tokens;
	int tokensPointer;
	char* bytecode;
	int bytecodePointer;
	char** values;
	int valuePointer;
		
}Program;

void Compile(char* source);
void CompileFile(char * sourcePath);

enum ByteCode
{
	Bytecode_Halt,
	Bytecode_GoTo,
	Bytecode_Call,

	Bytecode_ClearMemory,
	Bytecode_ClearStorage,

	Bytecode_PushByte,
	Bytecode_PushInt,
	Bytecode_PushFloat,

	Bytecode_PopByte,
	Bytecode_PopInt,
	Bytecode_PopFloat,

	Bytecode_StoreByte,
	Bytecode_StoreInt,
	Bytecode_StoreFloat,

	Bytecode_LoadByte,
	Bytecode_LoadInt,
	Bytecode_LoadFloat,

	Bytecode_SetByte,
	Bytecode_SetInt,
	Bytecode_SetFloat,

	Bytecode_AddByte,
	Bytecode_AddInt,
	Bytecode_AddFloat,

	Bytecode_SubtractByte,
	Bytecode_SubtractInt,
	Bytecode_SubtractFloat,

	Bytecode_MultiplyByte,
	Bytecode_MultiplyInt,
	Bytecode_MultiplyFloat,

	Bytecode_DivideByte,
	Bytecode_DivideInt,
	Bytecode_DivideFloat,

	Bytecode_Byte2Int,
	Bytecode_Byte2Float,
	Bytecode_Int2Float,
	Bytecode_Float2Int,

	Bytecode_SmallerThan,
	Bytecode_BiggerThan,
	Bytecode_IsEqual,

	Bytecode_If,
	Bytecode_IfNot,

	Bytecode_And,
	Bytecode_Or,
	Bytecode_Not,
};

enum Tokens
{
	Public,
	Private,
	Protected,
	Static,

	Namespace,
	Using,
	Class,
	Fullstop,

	Method,
	Function,

	Return,
	Struct,
	Array,

	While,
	For,

	If,
	Elif,
	Else,

	New,
	Assign,
	Void,
	Integer,
	Float,
	String,
	Char,
	Variable,
	Comma,

	Add,
	Subtract,
	Asterisk,
	Divide,

	Equal,
	IsNot,
	MoreThan,
	LessThan,

	Semicolon,
	OpenBracket,
	CloseBracket,
	OpenParentheses,
	CloseParentheses,
	OpenCurlyBracket,
	CloseCurlyBracket,

	Value
};