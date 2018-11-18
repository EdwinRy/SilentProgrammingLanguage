#include <stdlib.h>
#include "SilentLib.h"
#ifndef SILENT_VM
#define SILENT_VM

typedef enum SilentBytecode
{
    Halt = 0,
	Goto,
	Sweep,

	Call,
	Return,
	LoadDll,
	LoadDllFunc,
	FreeDll,
	CallDllFunc,

	Push1,
	Push2,
	Push4,
	Push8,

	Pop1,
	Pop2,
	Pop4,
	Pop8,

	Store1,
	Store2,
	Store4,
	Store8,

	Load1,
	Load2,
	Load4,
	Load8,

	StoreGlobal1,
	StoreGlobal2,
	StoreGlobal4,
	StoreGlobal8,

	LoadGlobal1,
	LoadGlobal2,
	LoadGlobal4,
	LoadGlobal8,

	//Alloc,
	//LoadPtr,
	//StorePtr,
	//FreePtr,
	//Free,
	//GetPtr,

    AddI1,
    AddI2,
    AddI4,
    AddI8,
    AddF4,
    AddF8,

    SubI1,
    SubI2,
    SubI4,
    SubI8,
    SubF4,
    SubF8,

    MulI1,
    MulI2,
    MulI4,
    MulI8,
    MulF4,
    MulF8,

    DivI1,
    DivI2,
    DivI4,
    DivI8,
    DivF4,
    DivF8,

	//Convert,
	SmallerThan,
	SmallerThanOrEqual,
	LargerThan,
	LargerThanOrEqual,
	Equal,
	NotEqual,

	If,
	IfNot,
	And,
	Or,
	Xor,
	Not
}SilentBytecode;

typedef enum SilentDataType
{
	INT8 = 0,
	UINT8,
	INT16,
	UINT16,
	INT32,
	UINT32,
	INT64,
	UINT64,
	FLOAT32,
	FLOAT64,
	POINTER,
	POINTER_LOCATION,
	UNDEFINED,
	UNDEFINED_END
}SilentDataType;

void SilentStartVM();

#endif //SILENT_VM