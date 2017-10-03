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

void deleteSilentMemory(SilentMemory * memory)
{
	free(memory->stack);
	free(memory->storage);
	free(memory);
}

void deleteSilentThread(SilentThread * thread)
{
	free(thread);
}

void executeSilentThread(SilentThread * thread)
{
	thread->running = 1;
	while(thread->running)
	{
		switch(thread->bytecode[thread->programCounter])
		{
			case SilentBytecode.Halt:
				thread->running = 0;
				break;
			
			case SilentBytecode.Goto:
				thread->programCounter = 
					*((unsigned int*)(&vm->bytecode[1 + thread->programCounter]));
				break;
			
			case SilentBytecode.Call: //Not yet implemented
				break;

			case SilentBytecode.ClearMemory:
				for(int i = 0; i < thread->memory->storagePointer; i++)
				{free(thread->memory->storage);}
				thread->memory->storagePointer = 0;
				break;

			case SilentBytecode.ClearStack:
				memset(thread->memory->stack, 0, thread->memory->stackPointer);
				thread->memory->stackPointer = 0;
				break;


			case SilentBytecode.Push1:
				thread->memory->stack[thread->memory->stackPointer++] = 
					thread->bytecode[++thread->programCounter];
				break;
				
			case SilentBytecode.Push4:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						4);
				thread->programCounter += 3;
				thread->memory->stackPointer += 4;
				break;

			case SilentBytecode.Push8:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						8);
				thread->programCounter += 7;
				thread->memory->stackPointer += 8;
				break;

			case SilentBytecode.PushX:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (4 + thread->programCounter),
						*((int*)(thread->bytecode + (1 + thread->programCounter))));
				thread->programCounter += 8;
				thread->memory->stackPointer += 8;
				break;
			
			
			
		}
	}
}
