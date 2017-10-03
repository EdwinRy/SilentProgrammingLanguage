#include "SilentVM.h"
SilentMemory* createSilentMemory(int storageSize, int stackSize)
{
	SilentMemory* memory = malloc(sizeof(SilentMemory));
	memory->storage = malloc(sizeof(char*)*storageSize);
	memory->stack = malloc(stackSize);
	memory->storagePointer = 0;
	memory->stackPointer = 0;
	memory->functionPointer = 0;
}

SilentThread* createSilentThread(SilentMemory* memory, char* bytecode)
{
	SilentThread* thread = malloc(sizeof(SilentThread));
	thread->bytecode = bytecode;
	thread->running = 0;
	thread->programCounter = 0;
	thread->memory = memory;
}
