
#pragma once

typedef struct SilentObject
{
	char* data;
	char marked;

}SilentObject;

typedef struct SilentMemory
{

	SilentObject* storage;
	SilentObject* stack;
	unsigned int storagePoiner;
	unsigned int stackPointer;

}SilentMemory;

typedef struct SilentThread
{
	char* bytecode;
	SilentMemory* memory;

}SilentThread;

typedef struct SilentVM
{
	char *bytecode;
	SilentThread** threads;

}SilentVM;

SilentObject* createSilentObject(char* data);
SilentMemory* createSilentMemory(int storageSize, int stackSize);
SilentThread* createSilentThread(char* bytecode, SilentMemory *memory);
SilentVM* createSilentVM(SilentThread *threads, int numberOfThreads);

void deleteSilentObject(SilentObject* object);
void deleteSilentMemory(SilentMemory* memory);
void deleteSilentThread(SilentThread* thread);
void deleteSilentVM(SilentVM* vm);