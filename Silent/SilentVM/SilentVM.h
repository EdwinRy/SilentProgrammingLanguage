#include <stdio.h>
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
	unsigned int threadID;
	SilentMemory* memory;

}SilentThread;

typedef struct SilentVM
{
	char *bytecode;
	SilentThread** threads;
	unsigned int defaultThread;
	unsigned int threadPointer;

}SilentVM;

SilentObject* createSilentObject(char* data);
SilentMemory* createSilentMemory(int storageSize, int stackSize);
SilentThread* createSilentThread(char* bytecode, SilentMemory *memory);
SilentVM* createSilentVM(SilentThread **threads, int numberOfThreads);

void deleteSilentObject(SilentObject* object);
void deleteSilentMemory(SilentMemory* memory);
void deleteSilentThread(SilentThread* thread);
void deleteSilentVM(SilentVM* vm);

void loadProgramToSilentVM(SilentVM* vm, char* program);
void attachThreadToSilentVM(SilentVM* vm, SilentThread* thread);
void loadProgramToSilentThread(SilentThread* thread, char* program);
void detachSilentThread(SilentVM* vm, unsigned int threadID);
void SetDefaultSilentThread(SilentVM* vm, unsigned int threadID);
void executeSilentThread(SilentVM* vm, unsigned int threadID);
void startSilentVM(SilentVM *vm);