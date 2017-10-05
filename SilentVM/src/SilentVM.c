#include "SilentVM.h"
#include <string.h>
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
			case Halt:
				thread->running = 0;
				break;
			
			case Goto:
				thread->programCounter = 
					*((unsigned int*)(&thread->bytecode[1 + thread->programCounter]));
				break;
			
			case Call: //Not yet implemented
				break;

			case ClearMemory:
				for(int i = 0; i < thread->memory->storagePointer; i++)
				{free(thread->memory->storage);}
				thread->memory->storagePointer = 0;
				break;

			case ClearStack:
				memset(thread->memory->stack, 0, thread->memory->stackPointer);
				thread->memory->stackPointer = 0;
				break;


			case Push1:
				thread->memory->stack[thread->memory->stackPointer++] = 
					thread->bytecode[++thread->programCounter];
				break;
				
			case Push4:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						4);
				thread->programCounter += 3;
				thread->memory->stackPointer += 4;
				break;

			case Push8:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						8);
				thread->programCounter += 7;
				thread->memory->stackPointer += 8;
				break;

			case PushX:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (4 + thread->programCounter),
						*((int*)(thread->bytecode + (1 + thread->programCounter))));
				thread->programCounter += 8;
				thread->memory->stackPointer += 8;
				break;
			
			
			case Pop1:
				break;

			case Pop4:
				break;
					
			case Pop8:
				break;

			case PopX:
				break;

			case Store1:
				break;

			case Store4:
				break;

			case Store8:
				break;

			case StoreX:
				break;

			case Load1:
				break;

			case Load4:
				break;

			case Load8:
				break;

			case LoadX:
				break;

			case Save1:
				break;

			case Save4:
				break;

			case Save8:
				break;

			case SaveX:
				break;

			case AddByte:
				break;

			case AddInt:
				break;

			case AddLong:
				break;

			case AddFloat:
				break;

			case AddDouble:
				break;

			case SubByte:
				break;

			case SubInt:
				break;

			case SubLong:
				break;

			case SubFloat:
				break;

			case SubDouble:
				break;
			
		}
	}
}
