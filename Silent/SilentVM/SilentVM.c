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

SilentVM * createSilentVM(SilentThread ** threads, int numberOfThreads)
{
	SilentVM* vm = malloc(sizeof(SilentVM));
	vm->threads = malloc(sizeof(SilentThread*) * numberOfThreads);
	vm->threadPointer = 0;
	vm->defaultThread = 0;
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
	free(vm);
}



void loadProgramToSilentVM(SilentVM * vm, char * program)
{
	vm->bytecode = program;
}

void attachThreadToSilentVM(SilentVM * vm, SilentThread * thread)
{
	if (vm->threadPointer != 0) 
	{
		for (unsigned int i = 0; i < vm->threadPointer; i++) 
		{
			if (vm->threads[i] == NULL) 
			{
				thread->threadID = i;
				vm->threads[i] = thread;
				break;
			}
		}
		vm->threads[vm->threadPointer] = thread;
		thread->threadID = vm->threadPointer;
		vm->threadPointer++;
	}

	else 
	{
		vm->threads[vm->threadPointer] = thread;
		thread->threadID = vm->threadPointer;
		vm->threadPointer++;
	}
}

void loadProgramToSilentThread(SilentThread * thread, char * program)
{
	thread->bytecode = program;
}

void detachSilentThread(SilentVM * vm, unsigned int threadID)
{
	vm->threads[threadID] = NULL;
}

void SetDefaultSilentThread(SilentVM * vm, unsigned int threadID)
{
}

void executeSilentThread(SilentVM * vm, unsigned int threadID)
{

}

void startSilentVM(SilentVM * vm)
{
}