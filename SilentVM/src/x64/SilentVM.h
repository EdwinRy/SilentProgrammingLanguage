#include <stdlib.h>
#ifndef SILENT_VM
#define SILENT_VM

typedef enum SilentBytecode
{
	Halt,
	Goto,
	Sweep,
    UseGlobal,
    EndGlobal,
	Call,
	Return,

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

	FREE,

	AddByte,
	AddShort,
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	SubByte,
	SubShort,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	MulByte,
	MulShort,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	DivByte,
	DivShort,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	ByteToInt,
	ByteToShort,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	IntToByte,
	IntToShort,
	IntToLong,
	IntToFloat,
	IntToDouble,

	FloatToByte,
	FloatToShort,
	FloatToInt,
	FloatToLong,
	FloatToDouble,

	DoubleToByte,
	DoubleToShort,
	DoubleToInt,
	DoubleToLong,
	DoubleToFloat,

	LongToByte,
	LongToShort,
	LongToInt,
	LongToFloat,
	LongToDouble,
	
	SmallerThanByte,
	SmallerThanShort,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	LargerThanByte,
	LargerThanShort,
	LargerThanInt,
	LargerThanLong,
	LargerThanFloat,
	LargerThanDouble,

	EqualByte,
	EqualShort,
	EqualInt,
	EqualLong,
	EqualFloat,
	EqualDouble,

	If,
	IfNot
}SilentBytecode;

typedef struct SilentMemoryBlock
{
	char* 	data;
	char 	occupied;
}SilentMemoryBlock;

typedef struct SilentMemory
{
	char* 				stack;
	SilentMemoryBlock* 	heap;
	unsigned long*		stackFrame;
    unsigned long 		stackPointer;
	unsigned long 		framePointer;
	unsigned long		stackFramePointer;
}SilentMemory;

typedef struct SilentVM
{
	SilentMemory* 	memory;
	char* 			program;
	char 			running;
	unsigned int 	programCounter;
}SilentVM;

SilentMemory* createSilentMemory(
	unsigned int stackBufferSize, //How much to allocate each realloc 
	unsigned int heapBufferSize, //same as stackBuffer 
	unsigned int stackFrameSize
);

SilentVM* createSilentVM(SilentMemory* memory, char* program);
void silentVMStart(SilentVM* vm);

void deleteSilentMemory(SilentMemory* memory);
void deleteSilentVM(SilentVM* vm);

#endif //SILENT_VM

