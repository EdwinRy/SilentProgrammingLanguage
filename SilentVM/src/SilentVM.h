#include <stdlib.h>
//#include "SilentVector.h"
#include "SilentGB.h"
#ifndef SILENT_VM
#define SILENT_VM
//<instruction> <argument> <argument>
//Argument types:
//X = number
//Y = string
typedef enum SilentBytecode
{
	//Stops the execution of the program
	Halt,
	//Moves the program counter to X
	Goto, //X - Byte
	//Call native subroutine
	CallSys,//X - 4 bytes

	//Disable storage pointer
    UseGlobal,
	//Enable storage pointer
    EndGlobal,

	//Call silent subrouting
	Call,//X - 4 bytes
	//Return back from subroutine
	Return,
	
	//ClearMemory,
	//ClearStack,
	
	//Pushes values onto the stack (CPU stack)
	Push1, //X - 1 byte value
	Push4, //X - 4 byte value
	Push8, //X - 8 byte value
	PushX, //X - X byte value, X - value
	
	//Pops values from the stack
	Pop1, //X - 1 byte value
	Pop4, //X - 4 byte value
	Pop8, //X - 8 byte value
	PopX, //X - X byte value, X - value

	//Store value in dedicated memory
	Store1, //X - memory address
	Store4, //X - memory address
	Store8, //X - memory address
	StoreX, //X - X value size, X - memory address
	
	//Load value from memory
	Load1, //X - memory address
	Load4, //X - memory address
	Load8, //X - memory address
	LoadX, //X - X value size, X - memory address

	//Allocate space for a value
	Alloc1, //X - memory address
	Alloc4, //X - memory address
	Alloc8, //X - memory address
	AllocX, //X - memory address

	//Return a pointer to X and push it to stack
	GetPtr, //X - memory address

	//Push 1 byte from the pointer
	LoadPtr1, 
	//Push 4 byte from the pointer
	LoadPtr4, 
	//Push 8 byte from the pointer
	LoadPtr8, 
	//Push X bytes from the pointer
	LoadPtrX, //X - value size

	//Change 1 byte at a pointer
	EditPtr1,
	//Change 4 bytes at a pointer
	EditPtr4,
	//Change 8 bytes at a pointer
	EditPtr8,
	//Change X bytes at a pointer
	EditPtrX, //X - value size

	//Free a value at address
	FREE, //X - memory address

	//Add numbers on the stack together
	AddByte,
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	//Subtract values on the stack
	//stack before:    	stack after
	// 2				0
	// 4				2
	//Operation is applied to the first
	//pushed value
	SubByte,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	//Multiply numbers on the stack
	MulByte,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	//Divide numbers on the stack
	//in the same order as subtraction
	DivByte,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	//Casting bytes to other data
	ByteToInt,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	//Casting integers to other data
	IntToByte,
	IntToLong,
	IntToFloat,
	IntToDouble,

	//Casting floats to other data
	FloatToByte,//
	FloatToInt,//
	FloatToLong,//
	FloatToDouble,//

	//Casting doubles to other data
	DoubleToByte,//
	DoubleToInt,//
	DoubleToLong,//
	DoubleToFloat,//

	//Cast long value to other data types
	LongToByte,//
	LongToInt,//
	LongToFloat,//
	LongToDouble,//
	
	//Smaller than operators
	//Same order as with subtraction
	SmallerThanByte,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	//Larger than operators
	//Same order as subtraction
	BiggerThanByte,
	BiggerThanInt,
	BiggerThanLong,
	BiggerThanFloat,
	BiggerThanDouble,

	//Equality operators
	EqualByte,
	EqualInt,
	EqualLong,
	EqualFloat,
	EqualDouble,

	//All of the comparison operators return either
	// a 1 - true or 0 - false to the stack
	
	//If theres a 1 on the stack
	If,
	//If the isn't one
	IfNot
}SilentBytecode;

typedef struct SilentMemory
{
	//char** storage;
	silentBlock** storage;
	vector* storagePointers;
	vector* programCounters;
	char* stack;
	unsigned int storageSize;
	//How much to add to realloc size
	unsigned int reallocSize;
	unsigned int stackPointer;
}SilentMemory;

typedef struct SilentThread
{
	char* bytecode;
	char running;
	unsigned int programCounter;
	SilentMemory* memory;
	SilentGB* garbageCollector;
}SilentThread;

//Create structures
SilentMemory* createSilentMemory(int storageSize, int stackSize);
SilentThread* createSilentThread(SilentMemory* memory,
	char* bytecode);

//Virtual machine functions
void deleteSilentMemory(SilentMemory* memory);
void deleteSilentThread(SilentThread* thread);
void executeSilentThread(SilentThread* thread);

#endif //SILENT_VM

