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
	return memory;
}

SilentThread* createSilentThread(SilentMemory* memory, char* bytecode)
{
	SilentThread* thread = malloc(sizeof(SilentThread));
	thread->bytecode = bytecode;
	thread->running = 0;
	thread->programCounter = 0;
	thread->memory = memory;
	return thread;
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
	long lreg;
	while(thread->running)
	{
		switch(thread->bytecode[thread->programCounter])
		{
			case Halt:
				thread->running = 0;
				break;
			
			case Goto:
				thread->programCounter = 
					*((unsigned long*)(&thread->bytecode[1 + thread->programCounter]));
				break;
			
			case Call: //Not yet implemented
				break;

			//case ClearMemory://
			//	for(int i = 0; i < thread->memory->storagePointer; i++)
			//	{free(thread->memory->storage);}
			//	thread->memory->storagePointer = 0;
			//	break;

			//case ClearStack://
			//	memset(thread->memory->stack, 0, thread->memory->stackPointer);
			//	thread->memory->stackPointer = 0;
			//	break;


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
				lreg = *((long*)(thread->bytecode + (++thread->programCounter)));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + 8 + thread->programCounter,
						lreg);
				thread->programCounter += (7+lreg);
				thread->memory->stackPointer += lreg;
				break;
			
			
			case Pop1:
				thread->memory->stackPointer--;
				break;

			case Pop4:
				thread->memory->stackPointer-=4;
				break;
					
			case Pop8:
				thread->memory->stackPointer-=8;
				break;

			case PopX:
				thread->memory->stackPointer-=
					*(long*)(thread->bytecode + (++thread->programCounter));
				thread->programCounter += 7;
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

			case Alloc1://
				thread->memory->storage[thread->memory->storagePointer]
					= malloc(1);
				break;

			case Alloc4://
				thread->memory->storage[thread->memory->storagePointer]
					= malloc(4);
				break;

			case Alloc8://
				thread->memory->storage[thread->memory->storagePointer]
					= malloc(8);
				break;

			case AllocX://
				thread->memory->storage[thread->memory->storagePointer]
					= malloc(*(long*)(thread->bytecode + (++thread->programCounter)));
				thread->programCounter += 7;
				break;

			case FREE://
				free(thread->memory->storage[thread->memory->storagePointer--]);
				break;

			case LoadPtr:
				break;

			case EditPtr1:
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
			
			case MulByte:
				break;

			case MulInt:
				break;

			case MulLong:
				break;

			case MulFloat:
				break;

			case MulDouble:
				break;

			case DivByte:
				break;

			case DivInt:
				break;

			case DivLong:
				break;

			case DivFloat:
				break;

			case DivDouble:
				break;

			case ByteToInt:
				break;

			case ByteToLong:
				break;

			case ByteToFloat:
				break;

			case ByteToDouble:
				break;

			case IntToByte:
				break;

			case IntToFloat:
				break;

			case IntToLong:
				break;

			case IntToDouble:
				break;

			case FloatToInt:
				break;

			case FloatToDouble:
				break;

			case SmallerThan:
				break;

			case BiggerThan:
				break;

			case Equal:
				break;

			case If:
				break;

			case IfNot:
				break;	
		}
		thread->programCounter++;
	}
}
