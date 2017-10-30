#include <stdlib.h>

#ifndef SILENTVM
#define SILENTVM
typedef enum SilentBytecode
{
	Halt,
	Goto,
	Call,//
	
	//ClearMemory,
	//ClearStack,
	
	Push1,
	Push4,
	Push8,
	PushX,
	
	Pop1,
	Pop4,
	Pop8,
	PopX,

	Store1,
	Store4,
	Store8,
	StoreX,
	
	Load1,
	Load4,
	Load8,
	LoadX,

	Alloc1,
	Alloc4,
	Alloc8,
	AllocX,

	FREE,

	/*
	LoadPtr,
	EditPtr1,
	EditPtr4,
	EditPtr8,
	EditPtrX,
	*/

	AddByte,
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	SubByte,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	MulByte,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	DivByte,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	ByteToInt,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	IntToByte,
	IntToLong,
	IntToFloat,
	IntToDouble,

	FloatToByte,//
	FloatToInt,//
	FloatToLong,//
	FloatToDouble,//

	DoubleToByte,//
	DoubleToInt,//
	DoubleToLong,//
	DoubleToFloat,//
	
	SmallerThanByte,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	BiggerThanByte,
	BiggerThanInt,
	BiggerThanLong,
	BiggerThanFloat,
	BiggerThanDouble,

	EqualByte,
	EqualInt,
	EqualLong,
	EqualFloat,
	EqualDouble,
	
	If,
	IfNot
}SilentBytecode;

typedef struct SilentMemory
{
	char** storage;
	char* stack;
	unsigned int storagePointer;
	unsigned int stackPointer;
	unsigned int functionPointer;
}SilentMemory;

typedef struct SilentThread
{
	char* bytecode;
	char running;
	unsigned int programCounter;
	SilentMemory* memory;
}SilentThread;

//Create structures
SilentMemory* createSilentMemory(int storageSize, int stackSize);
SilentThread* createSilentThread(SilentMemory* memory, char* bytecode);

//Virtual machine functions
void deleteSilentMemory(SilentMemory* memory);
void deleteSilentThread(SilentThread* thread);
void executeSilentThread(SilentThread* thread);

#endif //SILENTVM
