#include <stdlib.h>
#include "SilentVector.h"
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
	Push,
	Pop,
	Store,
	Load,
	StoreGlobal,
	LoadGlobal,
	Alloc,
	LoadPtr,
	StorePtr,
	FreePtr,
	Free,
	GetPtr,
	Add,
	Sub,	
	Mul,	
	Div,
	Convert,
	SmallerThan,
	LargerThan,
	Equal,
	If,
	IfNot,
	And,
	Or,
	Not
}SilentBytecode;

typedef struct SilentMemoryBlock
{
	char 	occupied;
	char 	marked;
	char* 	data;
}SilentMemoryBlock;

typedef struct SilentMemory
{
	char* 					stack;
	unsigned long long		stackSize;
    unsigned long long		stackPointer;
	unsigned long long		framePointer;

	SilentVector* 			heap;
	char					freeHeapSpace;
	unsigned long long		heapPtr;
	SilentVector*			stackTypes;
	SilentVector*			stackFrame;
}SilentMemory;

typedef struct SilentGC
{
    SilentMemory* memory;
}SilentGC;

typedef struct SilentDllProc
{
	char* procName;
	LibFunc addr;
}SilentDllProc;

typedef struct SilentDll
{
	SilentDllProc* procs;
	char* name;
	void* addr;
	unsigned long long count;
}SilentDll;

typedef struct SilentVM
{
	SilentMemory* 		memory;
	SilentGC*			gc;
	SilentDll*			dlls;
	unsigned long long	dllCount;
	char* 				program;
	unsigned long long	programCounter;
	char 				running;
}SilentVM;

SilentMemory* createSilentMemory(
	unsigned int stackBufferSize, //How much to allocate each realloc 
	unsigned int heapBufferSize //same as stackBuffer 
);

SilentVM* createSilentVM(SilentMemory* memory, char* program, SilentGC* gc);
void silentVMStart(SilentVM* vm);

void deleteSilentMemory(SilentMemory* memory);
void deleteSilentVM(SilentVM* vm);

SilentGC* createSilentGC(SilentMemory* memory);
void SilentSweep(SilentGC* gc);
void SilentMark(SilentGC* gc);
void SilentFree(SilentGC* gc, unsigned long long ptr);
long SilentAlloc(SilentGC* gc, unsigned long long size);
void deleteSilentGC(SilentGC* gc);

#endif //SILENT_VM

