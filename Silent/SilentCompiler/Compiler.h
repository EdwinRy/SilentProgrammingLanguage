#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct assembly {
	char* code;
	long long codeSize;
	char* bytecode;
	char* bytecodeLength;
	char* outputFile;
}assembly;

void Compile(char* source);
void CompileAssembly(assembly* asm);

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
