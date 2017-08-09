#pragma once

void Compile(char* source) 
{

}

void CompileFile() 
{

}



enum ByteCode
{
	Halt,
	GoTo,
	Call,

	ClearMemory,
	ClearStorage,

	PushByte,
	PushInt,
	PushFloat,

	PopByte,
	PopInt,
	PopFloat,

	StoreByte,
	StoreInt,
	StoreFloat,

	LoadByte,
	LoadInt,
	LoadFloat,

	SetByte,
	SetInt,
	SetFloat,

	AddByte,
	AddInt,
	AddFloat,

	SubtractByte,
	SubtractInt,
	SubtractFloat,

	MultiplyByte,
	MultiplyInt,
	MultiplyFloat,

	DivideByte,
	DivideInt,
	DivideFloat,

	Byte2Int,
	Byte2Float,
	Int2Float,
	Float2Int,

	SmallerThan,
	BiggerThan,
	Equal,

	If,
	IfNot,

	And,
	Or,
	Not,
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