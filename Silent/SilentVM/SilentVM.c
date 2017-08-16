#include "SilentVM.h"

SilentObject * createSilentObject(char * data)
{
	SilentObject* object = malloc(sizeof(SilentObject));
	object->data = data;
	object->marked = 0;
	return object;
}

SilentMemory* createSilentMemory(int storageSize, int stackSize)
{
	SilentMemory* memory = malloc(sizeof(SilentMemory));
	memory->stack = malloc(stackSize);
	memory->storage = malloc(storageSize);
	memory->stackPointer = 0;
	memory->storagePoiner = 0;
}

SilentThread * createSilentThread(char * bytecode, SilentMemory * memory)
{
	SilentThread* thread = malloc(sizeof(SilentThread));
	thread->bytecode = bytecode;
	thread->memory = memory;
	return thread;
}

SilentVM * createSilentVM(SilentThread * threads, int numberOfThreads)
{
	SilentVM* vm;
	vm->threads = malloc(sizeof(SilentThread*) * numberOfThreads);
	return vm;
}

void deleteSilentObject(SilentObject * object)
{
	free(object);
}

void deleteSilentMemory(SilentMemory * memory)
{
	free(memory->stack);
	free(memory->storage);
	free(memory);
}

void deleteSilentThread(SilentThread * thread)
{
	deleteSilentMemory(thread->memory);
	free(thread);
}

void deleteSilentVM(SilentVM * vm)
{
	deleteSilentThread(vm->threads);
	free(SilentVM);
}
