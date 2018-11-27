#include <stdlib.h>
#include "SilentLib.h"
#ifndef SILENT_VM
#define SILENT_VM

typedef enum SilentBytecode
{
    Halt = 0,
	Goto,

	Call,
	Return,
	LoadDll,//
	LoadDllFunc,//
	FreeDll,//
	CallDllFunc,//

	Push1,
	Push2,
	Push4,
	Push8,
	PushX,

	Pop1,
	Pop2,
	Pop4,
	Pop8,
	PopX,

	Store1,
	Store2,
	Store4,
	Store8,
	StoreX,

	Load1,
	Load2,
	Load4,
	Load8,
	LoadX,

	StoreGlobal1,
	StoreGlobal2,
	StoreGlobal4,
	StoreGlobal8,
	StoreGlobalX,

	LoadGlobal1,
	LoadGlobal2,
	LoadGlobal4,
	LoadGlobal8,
	LoadGlobalX,

	Alloc1,
	Alloc2,
	Alloc4,
	Alloc8,
	AllocX,

	LoadPtr1,
	LoadPtr2,
	LoadPtr4,
	LoadPtr8,
	LoadPtrX,

	StorePtr1,
	StorePtr2,
	StorePtr4,
	StorePtr8,
	StorePtrX,
	Free,
	GetPtr,

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

typedef struct SilentMemoryBlock
{
	char occupied;
	char marked;
	char* data;
}SilentMemoryBlock;

typedef struct SilentGC
{
	SilentMemoryBlock* heap;
	unsigned long long heapPtr;
	unsigned long long heapSize;
	unsigned long long lastFree;
}SilentGC;

void SilentStartVM();
unsigned long long SilentAlloc(SilentGC* gc, unsigned long long size);
void SilentFree(SilentGC* gc, unsigned long long pos);

#endif //SILENT_VM