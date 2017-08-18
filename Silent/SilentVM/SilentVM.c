#include "SilentVM.h"
#define BYTECODE_HALT 0
#define BYTECODE_GOTO 1
#define BYTECODE_CALL 2

#define BYTECODE_CLEAR_MEMORY 3
#define BYTECODE_CLEAR_STACK 4

#define BYTECODE_PUSH_BYTE 5
#define BYTECODE_PUSH_INT 6
#define BYTECODE_PUSH_FLOAT 7

#define BYTECODE_POP_BYTE 8
#define BYTECODE_POP4 9

#define BYTECODE_STORE_BYTE 10
#define BYTECODE_STORE_INT 11
#define BYTECODE_STORE_FLOAT 12

#define BYTECODE_LOAD_BYTE 13
#define BYTECODE_LOAD_INT 14
#define BYTECODE_LOAD_FLOAT 15

#define BYTECODE_ADD_BYTE 16
#define BYTECODE_ADD_INT 17
#define BYTECODE_ADD_FLOAT 18

#define BYTECODE_SUB_BYTE 19
#define BYTECODE_SUB_INT 20
#define BYTECODE_SUB_FLOAT 21

#define BYTECODE_MUL_BYTE 22
#define BYTECODE_MUL_INT 23
#define BYTECODE_MUL_FLOAT 24

#define BYTECODE_DIV_BYTE 25
#define BYTECODE_DIV_INT 26
#define BYTECODE_DIV_FLOAT 27

#define BYTECODE_BYTE_TO_INT 28
#define BYTECODE_INT_TO_BYTE 29
#define BYTECODE_INT_TO_FLOAT 30
#define BYTECODE_FLOAT_TO_INT 31

#define BYTECODE_SMALLER_THAN 32
#define BYTECODE_BIGGER_THAN 33
#define BYTECODE_EQUAL 34

#define BYTECODE_IF 35
#define BYTECODE_IF_NOT 36


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
	thread->programCounter = 0;
	thread->bytecodeLength = strlen(bytecode);
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
	vm->defaultThread = threadID;
}

void executeSilentThread(SilentVM * vm, unsigned int threadID)
{
	SilentThread* thread = vm->threads[threadID];
	thread->running = 1;

	while (thread->running)
	{

		switch (thread->bytecode[thread->programCounter])
		{
		case BYTECODE_HALT:
			thread->running = 0;
			break;

		case BYTECODE_GOTO:
			vm->threadPointer++;
			thread->programCounter = *((unsigned int*)(vm->bytecode[vm->threadPointer]));
			vm->threadPointer--;
			break;

		case BYTECODE_CALL:
			//implement native function calls
			break;



		case BYTECODE_CLEAR_MEMORY:
			memset(thread->memory->storage, 0, thread->memory->storagePoiner);
			thread->memory->storagePoiner = 0;
			break;

		case BYTECODE_CLEAR_STACK:
			memset(thread->memory->stack, 0, thread->memory->stackPointer);
			break;



		case BYTECODE_PUSH_BYTE:
			break;

		case BYTECODE_PUSH_INT:
			break;

		case BYTECODE_PUSH_FLOAT:
			break;



		case BYTECODE_POP_BYTE:
			break;

		case BYTECODE_POP4:
			break;



		case BYTECODE_STORE_BYTE:
			break;

		case BYTECODE_STORE_INT:
			break;

		case BYTECODE_STORE_FLOAT:
			break;



		case BYTECODE_LOAD_BYTE:
			break;

		case BYTECODE_LOAD_INT:
			break;

		case BYTECODE_LOAD_FLOAT:
			break;



		case BYTECODE_ADD_BYTE:
			break;

		case BYTECODE_ADD_INT:
			break;

		case BYTECODE_ADD_FLOAT:
			break;



		case BYTECODE_SUB_BYTE:
			break;

		case BYTECODE_SUB_INT:
			break;

		case BYTECODE_SUB_FLOAT:
			break;



		case BYTECODE_MUL_BYTE:
			break;

		case BYTECODE_MUL_INT:
			break;

		case BYTECODE_MUL_FLOAT:
			break;



		case BYTECODE_DIV_BYTE:
			break;

		case BYTECODE_DIV_INT:
			break;

		case BYTECODE_DIV_FLOAT:
			break;



		case BYTECODE_BYTE_TO_INT:
			break;

		case BYTECODE_INT_TO_BYTE:
			break;

		case BYTECODE_INT_TO_FLOAT:
			break;

		case BYTECODE_FLOAT_TO_INT:
			break;



		case BYTECODE_SMALLER_THAN:
			break;

		case BYTECODE_BIGGER_THAN:
			break;

		case BYTECODE_EQUAL:
			break;



		case BYTECODE_IF:
			break;

		case BYTECODE_IF_NOT:
			break;
		}
		thread->programCounter++;
	}

}

void startSilentVM(SilentVM * vm)
{
	executeSilentThread(vm, vm->defaultThread);
}