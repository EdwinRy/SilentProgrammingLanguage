#include "SilentVM.h"
#include "SilentGB.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

//Allocate memory for the program
SilentMemory* createSilentMemory(int storageSize, int stackSize)
{
	SilentMemory* memory = malloc(sizeof(SilentMemory));
	memory->storagePointers = createVector(sizeof(int));
	memory->programCounters = createVector(sizeof(int));
	memory->stack = malloc(stackSize);
	memory->stackPointer = 0;
	memory->storage = malloc(sizeof(silentBlock*)*storageSize);
	memory->reallocSize = storageSize;
	memory->storageSize = storageSize;
	return memory;
}

//Create executing thread
SilentThread* createSilentThread(SilentMemory* memory, char* bytecode)
{
	SilentThread* thread = malloc(sizeof(SilentThread));
	thread->bytecode = bytecode;
	thread->running = 0;
	thread->programCounter = 0;
	thread->memory = memory;
	return thread;
}

//Delete program's allocated space
void deleteSilentMemory(SilentMemory * memory)
{
	free(memory->stack);
	free(memory->storage);
	free(memory);
}

//Delete program's thread
void deleteSilentThread(SilentThread * thread)
{
	free(thread);
}

void executeSilentThread(SilentThread * thread)
{
	char breg = 0;
	int ireg = 0;
	float freg = 0;
	long lreg = 0;
	double dreg = 0;
	thread->running = 1;
	SilentMemory* memory = thread->memory;
	int localStoragePointer = 0;
	int* storagePointer = memory->storagePointers->integers;
	int altStoragePointer = 0;
	int* lastPC = memory->programCounters->integers;

	while(thread->running)
	{
		switch(thread->bytecode[thread->programCounter])
		{
			//Stops the execution of the program
			case Halt:
				//printf("halt\n");
				thread->running = 0;
			break;
			
			//Changes the program pointer based on bytecode
			case Goto:
				//printf("goto\n");
				thread->programCounter++;
				thread->programCounter = 
					*((unsigned int*)(thread->bytecode + (thread->programCounter)));
				thread->programCounter--;
			break;
			
			//Will be used to call library functions
			case CallSys: //Not yet implemented
			break;

			//Disable storage pointer
			case UseGlobal:
				altStoragePointer = 0;
			break;

			//Call silent subrouting
			case EndGlobal:
				altStoragePointer = *storagePointer;
			break;

			case Call:
				//printf("call\n");
				vectorInsert(memory->storagePointers,&localStoragePointer,0);
				vectorInsert(memory->programCounters,&thread->programCounter,0);
				altStoragePointer = *storagePointer;
				thread->programCounter++;
				thread->programCounter = 
					*((unsigned int*)(thread->bytecode + (thread->programCounter)));
				thread->programCounter--;
			break;

			case Return:
				//printf("return\n");
				thread->programCounter = (*lastPC) + 4;
				vectorRemove(memory->storagePointers,0);
				vectorRemove(memory->programCounters,0);
				localStoragePointer = *storagePointer;
				altStoragePointer = *storagePointer;
			break;

			//Pushes 1 byte of data to the stack
			case Push1:
				thread->memory->stack[thread->memory->stackPointer++] = 
					thread->bytecode[++thread->programCounter];
			break;
				
			//Pushes 4 bytes of data to the stack
			case Push4:
				//printf("push4\n");
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						4);
				thread->programCounter += 3;
				thread->memory->stackPointer += 4;
			break;
			
			//Pushes 8 bytes of data to the stack
			case Push8:
				//printf("push8\n");
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						8);
				thread->programCounter += 7;
				thread->memory->stackPointer += 8;
			break;

			//Pushes X (in bytecode) bytes of data to the stack
			case PushX:
				//printf("pushx\n");
				lreg = *((int*)(thread->bytecode + (++thread->programCounter)));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + 4 + thread->programCounter,
						lreg);
				thread->programCounter += (3+lreg);
				thread->memory->stackPointer += lreg;
			break;
			
			//Decreases the stack pointer by 1
			case Pop1:
				thread->memory->stackPointer--;
			break;
			
			//Decreases the stack pointer by 4
			case Pop4:
				//printf("pop4\n");
				thread->memory->stackPointer-=4;
			break;
					
			//Decreases the stack pointer by 8
			case Pop8:
				thread->memory->stackPointer-=8;
			break;

			//Decreases the stack pointer by X (in bytecode)
			case PopX:
				thread->memory->stackPointer-=
					*(int*)(thread->bytecode + (++thread->programCounter));
				thread->programCounter += 3;
			break;

			//Saves 1 byte from the stack to allocated space
			case Store1:
				memcpy(			
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					thread->memory->stack + (--thread->memory->stackPointer),
					1
				);
				thread->programCounter += 3;
			break;

			//Saves 4 bytes from the stack to allocated space
			case Store4:
				//printf("store4\n");
				thread->memory->stackPointer -= 4;
				memcpy(
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					thread->memory->stack + (thread->memory->stackPointer),
					4
				);
				thread->programCounter += 3;
			break;

			//Saves 8 bytes from the stack to allocated space
			case Store8:
				thread->memory->stackPointer -= 8;
				memcpy(	
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					thread->memory->stack + (thread->memory->stackPointer),
					8
				);
				thread->programCounter += 3;		
			break;

			//Saves X bytes from stack to allocated space
			case StoreX:
				//printf("storex\n");
				//Data size
				ireg = *((int*)(thread->bytecode + (++thread->programCounter)))+1;
				thread->programCounter+=4;
				thread->memory->stackPointer-=ireg;
				memcpy(
					memory->storage[
						(*(int*)(thread->bytecode +(thread->programCounter))) +
						altStoragePointer
					]->data,
					thread->memory->stack + (++thread->memory->stackPointer),
					ireg);
				thread->programCounter += 3;
			break;
	
			//Copies 1 byte of data from storage onto the stack
			case Load1:
				memcpy
				(
					thread->memory->stack + (thread->memory->stackPointer++),
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					1
				);
				thread->programCounter += 3;
			break;

			//Copies 4 bytes of data from storage onto the stack
			case Load4:
				//printf("load4\n");
				memcpy
				(
					thread->memory->stack + (thread->memory->stackPointer),
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					4
				);
				thread->memory->stackPointer += 4;
				thread->programCounter += 3;
			break;

			//Copies 8 bytes of data from storage onto the stack
			case Load8:
				memcpy
				(
					thread->memory->stack + (thread->memory->stackPointer),
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					8
				);
				thread->memory->stackPointer += 8;
				thread->programCounter += 3;
			break;

			//Copies X bytes of data from storage onto the stack
			case LoadX:
				ireg = *((int*)(thread->bytecode + (++thread->programCounter)));
				thread->programCounter+=3;
				memcpy(
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					thread->memory->stack + (thread->memory->stackPointer),
					ireg);
				thread->programCounter += 3;
				memory->stackPointer += ireg;
			break;

			//Allocates 1 byte of data for the program
			case Alloc1:
				ireg = *(int*)(thread->bytecode +(++thread->programCounter));
				thread->programCounter += 3;
				ireg += altStoragePointer;
				if(ireg >= localStoragePointer)
					localStoragePointer = ireg + 1;
				while(ireg >= memory->storageSize)
				{
					memory->storageSize += memory->reallocSize;
					memory->storage = 
						realloc(memory->storage,memory->storageSize);
				}
				memory->storage[ireg] = malloc(sizeof(silentBlock));
				memory->storage[ireg]->data = malloc(1);
			break;

			//Allocates 4 bytes of data for the program
			case Alloc4:
				//printf("alloc4\n");
				ireg = *(int*)(thread->bytecode +(++thread->programCounter));
				thread->programCounter += 3;
				ireg += altStoragePointer;
				if(ireg >= localStoragePointer)
					localStoragePointer = ireg + 1;
				while(ireg >= memory->storageSize)
				{
					memory->storageSize += memory->reallocSize;
					memory->storage = 
						realloc(memory->storage,memory->storageSize);
				}
				memory->storage[ireg] = malloc(sizeof(silentBlock));
				memory->storage[ireg]->data = malloc(4);
			break;

			//Allocates 8 bytes of data for the program
			case Alloc8:
				ireg = *(int*)(thread->bytecode +(++thread->programCounter));
				thread->programCounter += 3;
				ireg += altStoragePointer;
				if(ireg >= localStoragePointer)
					localStoragePointer = ireg + 1;
				while(ireg >= memory->storageSize)
				{
					memory->storageSize += memory->reallocSize;
					memory->storage = 
						realloc(memory->storage,memory->storageSize);
				}
				memory->storage[ireg] = malloc(sizeof(silentBlock));
				memory->storage[ireg]->data = malloc(8);
			break;

			//Allocates X bytes of data for the program
			case AllocX:
				ireg = *(int*)(thread->bytecode + (++thread->programCounter)) + 1;
				thread->programCounter += 3;
				lreg = *(int*)(thread->bytecode + (++thread->programCounter));
				thread->programCounter += 3;
				lreg += altStoragePointer;
				if(lreg >= localStoragePointer)
					localStoragePointer = lreg + 1;
				while(lreg >= memory->storageSize)
				{
					memory->storageSize += memory->reallocSize;
					memory->storage = 
						realloc(memory->storage,memory->storageSize);
				}
				
				memory->storage[lreg] = malloc(sizeof(silentBlock));
				memory->storage[lreg]->data = malloc(ireg);
			break;
			
			case GetPtr:
				//printf("getptr\n");
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						(int*)&memory->storage[
							*(int*)(thread->bytecode + (++thread->programCounter)) +
							altStoragePointer
						]->data,
						8);
				thread->programCounter += 3;
				thread->memory->stackPointer += 8;
			break;


			case LoadPtr1:
				memory->stackPointer-=8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
					(long*)lreg,1);
				memory->stackPointer+=1;
			break;

			case LoadPtr4:
				memory->stackPointer-=8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
					(long*)lreg,4);
				memory->stackPointer+=4;
			break;

			case LoadPtr8:
				memory->stackPointer-=8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
					(long*)lreg,8);
				memory->stackPointer+=8;
			break;

			case LoadPtrX:
				memory->stackPointer-=8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				ireg = *(int*)(thread->bytecode + (++thread->programCounter));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
					(long*)lreg,ireg);
				thread->programCounter += 3;
				memory->stackPointer += ireg;
			break;

			case EditPtr1:
				//printf("editptr1\n");
				memory->stackPointer -= 8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memory->stackPointer -= 1;
				memcpy(
					(void*)lreg,
					(memory->stack + (memory->stackPointer)),
					sizeof(char)
				);
			break;

			case EditPtr4:
				memory->stackPointer -= 8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memory->stackPointer -= 4;
				memcpy(
					(void*)lreg,
					memory->stack + (memory->stackPointer),
					sizeof(int)
				);
			break;

			case EditPtr8:
				memory->stackPointer -= 8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memory->stackPointer -= 8;
				memcpy(
					(void*)lreg,
					memory->stack + (memory->stackPointer),
					sizeof(long)
				);
			break;

			case EditPtrX:
				memory->stackPointer -= 8;
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				ireg = *(int*)(thread->bytecode +(++thread->programCounter));
				memory->stackPointer -= ireg;
				memcpy(
					(void*)lreg,
					memory->stack + (memory->stackPointer),
					ireg
				);
				thread->programCounter += 3;
			break;

			//Releases the lastly allocated storage
			case FREE://
				ireg = *(int*)(thread->bytecode +(++thread->programCounter)) +
					altStoragePointer;
				free(memory->storage[ireg]->data);
				free(memory->storage[ireg]);
				thread->programCounter += 3;
			break;

			//Adds together 2 bytes on the stack
			case AddByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) += 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Adds together 2 integers on the stack
			case AddInt:
				//printf("addint\n");
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) += 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Adds together 2 longs on the stack
			case AddLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) += 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Adds together 2 floats on the stack
			case AddFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) += 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Adds together 2 doubles on the stack
			case AddDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) += 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);

			break;

			//Subtracts the last number from the second last number on the stack
			case SubByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) -= 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubInt:
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) -= 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) -= 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) -= 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) -= 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);

			break;
			
			//Multiplies 2 bytes together
			case MulByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) *= 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Multiplies 2 integers together
			case MulInt:
				//printf("mulint\n");
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) *= 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Multiplies 2 longs together
			case MulLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) *= 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Multiplies 2 floats together
			case MulFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) *= 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Multiplies 2 doubles together
			case MulDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) *= 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Divides 2 bytes
			case DivByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) /= 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Divides 2 integers
			case DivInt:
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) /= 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Divides 2 longs
			case DivLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) /= 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Divides 2 floats
			case DivFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) /= 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Divides 2 doubles
			case DivDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) /= 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);
			break;

			//Byte to integer conversion
			case ByteToInt://untested
				breg = *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				ireg = (int)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&ireg, 
					4);
				thread->memory->stackPointer+=4;
			break;

			//Byte to long conversion
			case ByteToLong://untested
				breg =  *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				lreg = (long)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&lreg,
					8);
				thread->memory->stackPointer+=8;
			break;

			//Byte to float conversion
			case ByteToFloat://untested
				breg =  *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				freg = (float)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&freg,
					4);
				thread->memory->stackPointer+=4;
			break;

			//Byte to double conversion
			case ByteToDouble://untested
				breg =  *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				dreg = (double)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&dreg,
					8);
				thread->memory->stackPointer+=8;
			break;

			//Integer to byte conversion
			case IntToByte://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-=4));
				breg = (char)ireg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&breg,
					1);
				thread->memory->stackPointer++;

			break;

			//Integer to long conversion
			case IntToLong://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-=4));
				lreg = (long)ireg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&lreg,
					8);
				thread->memory->stackPointer+=8;
			break;
			
			//Integer to float conversion
			case IntToFloat://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-4));
				freg = (float)ireg;
				memcpy(
					thread->memory->stack + (thread->memory->stackPointer-4),
					&freg,
					4);
			break;

			//Integer to double conversion
			case IntToDouble://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-4));
				dreg = (double)ireg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer-4,
					&freg,
					8);
				thread->memory->stackPointer+=4;
			break;

			//Float to integer conversion
			case FloatToInt://untested

				break;

			//Float to double conversion
			case FloatToDouble://untested
				thread->memory->stackPointer+=4;				
			break;
			
			//Compare value of 2 bytes
			case SmallerThanByte:
				thread->memory->stackPointer--;
				if(*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) <
					*(char*)(thread->memory->stack + thread->memory->stackPointer))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case SmallerThanInt:
				//printf("smallerint\n");
				thread->memory->stackPointer-=7;
				if(*(int*)(thread->memory->stack + thread->memory->stackPointer-1) < 
					*(int*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}			
			break;

			//Compare value of 2 8 bytes
			case SmallerThanLong:
				thread->memory->stackPointer-=15;
				if(*(long*)(thread->memory->stack + (thread->memory->stackPointer-1)) < 
					*(long*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case SmallerThanFloat:
				thread->memory->stackPointer-=7;
				if(*(float*)(thread->memory->stack + (thread->memory->stackPointer-1)) < 
					*(float*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case SmallerThanDouble:
				thread->memory->stackPointer-=15;
				if(*(double*)(thread->memory->stack + (thread->memory->stackPointer-1)) < 
					*(double*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 bytes
			case BiggerThanByte:
				thread->memory->stackPointer--;
				if(*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(char*)(thread->memory->stack + thread->memory->stackPointer))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case BiggerThanInt:
				thread->memory->stackPointer-=7;
				if(*(int*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(int*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case BiggerThanLong:
				thread->memory->stackPointer-=15;
				if(*(long*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(long*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case BiggerThanFloat:
				thread->memory->stackPointer-=7;
				if(*(float*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(float*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case BiggerThanDouble:
				thread->memory->stackPointer-=15;
				if(*(double*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(double*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 bytes
			case EqualByte:
				thread->memory->stackPointer--;
				if(*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) == 
					*(char*)(thread->memory->stack + thread->memory->stackPointer))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case EqualInt:
				thread->memory->stackPointer-=7;
				if(*(int*)(thread->memory->stack + (thread->memory->stackPointer-1)) == 
					*(int*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case EqualLong:
				thread->memory->stackPointer-=15;
				if(*(long*)(thread->memory->stack + (thread->memory->stackPointer-1)) == 
					*(long*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case EqualFloat:
				thread->memory->stackPointer-=7;
				if((*(float*)(thread->memory->stack + (thread->memory->stackPointer-1))) == 
					(*(float*)(thread->memory->stack + thread->memory->stackPointer+3)))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case EqualDouble:
				thread->memory->stackPointer-=15;
				if(*(double*)(thread->memory->stack + (thread->memory->stackPointer-1)) ==
					*(double*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
			break;


			case If:
				//printf("if\n");
				if(*(char*)(thread->memory->stack + --thread->memory->stackPointer))
				{
					thread->programCounter++;
					thread->programCounter = 
					*((unsigned int*)(thread->bytecode + (thread->programCounter)));
					thread->programCounter--;
				}
				else
				{
					thread->programCounter += 4;
				}
			break;


			case IfNot:
				//printf("ifn\n");
				if(!(*(char*)(thread->memory->stack + (--thread->memory->stackPointer))))
				{
					thread->programCounter++;
					thread->programCounter = 
					*((unsigned int*)(thread->bytecode + (thread->programCounter)));
					thread->programCounter--;
				}
				else
				{
					thread->programCounter += 4;
				}
			break;	
		}
		//printf("stack 1st element:%u\n",(int)memory->stack[0]);
		//printf("programCounter:%i\n",(int)thread->programCounter);
		//printf("Which function:%i\n",(int)memory->storagePointers->dataCount);
		//getchar();
		thread->programCounter++;
	}
}
