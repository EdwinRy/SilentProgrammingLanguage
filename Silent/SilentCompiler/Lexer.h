#include <stdio.h>
#include <stdlib.h>
#pragma once
typedef struct Program
{
	char* source;
	unsigned long sourceLength;
	char* tokens;
	unsigned long tokensPointer;
	char* bytecode;
	unsigned long bytecodePointer;
	char** values;
	unsigned long valuePointer;
	unsigned long valuesAllocationPointer;

}Program;

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

void Tokenize(Program* program);