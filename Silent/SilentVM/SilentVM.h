#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct SilentMemory
{

	char** storage;
	char* stack;
	unsigned int storagePoiner;
	unsigned int stackPointer;
	unsigned int functionPointer;

}SilentMemory;

typedef struct SilentThread
{
	char* bytecode;
	char running;
	unsigned int bytecodeLength;
	unsigned int programCounter;
	unsigned int threadID;
	SilentMemory* memory;

}SilentThread;

typedef struct SilentVM
{
	char *bytecode;
	SilentThread** threads;
	unsigned int bytecodeLength;
	unsigned int defaultThread;
	unsigned int threadPointer;

}SilentVM;

SilentMemory* createSilentMemory(int storageSize, int stackSize);
SilentThread* createSilentThread(char* bytecode, SilentMemory *memory);
SilentVM* createSilentVM(SilentThread *thread, int numberOfThreads);

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
