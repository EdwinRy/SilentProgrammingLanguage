#include "SilentVM.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#ifdef __linux__
//Linux specific code
#include <dlfcn.h>
#elif _WIN32
//Windows specific code

#else
//Cross platform workaround
printf("Your system won't be able to suppord native libraries\n");
#endif

//Allocate memory for the program
SilentMemory* createSilentMemory(
	unsigned int stackS,
	unsigned int memStackS, 
	unsigned int heapS
)
{
	SilentMemory* memory 	= malloc(sizeof(SilentMemory));
	memory->stack 			= calloc(stackS,sizeof(char));
	memory->stackTop		= 0;
	memory->memStack 		= calloc(memStackS,sizeof(SilentMemoryBlock));
	memory->stackPointer	= 0;
	memory->framePointer	= 0;
	memory->memHeap			= calloc(heapS,sizeof(char));
	return memory;
}

//Create executing thread
SilentThread* createSilentThread(SilentMemory* memory, char* bytecode)
{
	SilentThread* thread 		= malloc(sizeof(SilentThread));
	thread->memory 				= memory;
	thread->bytecode 			= bytecode;
	thread->running 			= 0;
	thread->programCounter 		= 0;
	return thread;
}

//Delete program's allocated space
void deleteSilentMemory(SilentMemory * memory)
{
	free(memory->stack);
	free(memory->memStack);
	free(memory->memHeap);
	free(memory);
}

//Delete program's thread
void deleteSilentThread(SilentThread * thread)
{
	free(thread);
}

void executeSilentThread(SilentThread * thread)
{
	//Setup registers
	char 	breg = 0;
	int 	ireg = 0;
	float 	freg = 0;
	long 	lreg = 0;
	double 	dreg = 0;

	//Set thread state to running
	thread->running = 1;

	SilentMemory* 	memory 				= thread->memory;
	SilentGB* 		gb 					= thread->garbageCollector;
	int* 			storagePointer 		= memory->storagePointers->integers;
	int*			storageCount 		= malloc(sizeof(int));
	int* 			lastPC 				= memory->programCounters->integers;
	int 			localStoragePointer = 0;
	int 			altStoragePointer 	= 0;
	
	*storageCount = 0;

	//insert initial storage pointer of 0
	vectorInsert(memory->storagePointers,&localStoragePointer,0);
	vectorInsert(memory->programCounters,&thread->programCounter,0);

	while(thread->running)
	{
		switch(thread->bytecode[thread->programCounter])
		{
			//Stops the execution of the program
			case Halt:
				thread->running = 0;
			break;
			
			//Changes the program pointer based on bytecode
			case Goto:
				thread->programCounter++;
				thread->programCounter = 
					*((unsigned int*)(thread->bytecode + (thread->programCounter)));
				thread->programCounter--;
			break;
			
			case LoadSys: //Not yet implemented
			break;

			case CallSys: //Not yet implemented
			break;

			case GBSweep:
				silentSweep(gb,memory,storageCount);
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
				//silentSweep(gb,memory,storageCount);
				thread->programCounter = (*lastPC) + 4;
				localStoragePointer = storagePointer[0];
				altStoragePointer = storagePointer[0];
				vectorRemove(memory->storagePointers,0);
				vectorRemove(memory->programCounters,0);
			break;

			//Pushes 1 byte of data to the stack
			case Push1:
				memory->stack[memory->stackPointer++] = 
					thread->bytecode[++thread->programCounter];
			break;
				
			//Pushes 4 bytes of data to the stack
			case Push4:
				//printf("push4\n");
				memcpy(memory->stack + memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						4);
				thread->programCounter += 3;
				memory->stackPointer += 4;
			break;
			
			//Pushes 8 bytes of data to the stack
			case Push8:
				//printf("push8\n");
				memcpy(memory->stack + memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						sizeof(long));
				thread->programCounter += 7;
				memory->stackPointer += sizeof(long);
			break;

			//Pushes X (in bytecode) bytes of data to the stack
			case PushX:
				//printf("pushx\n");
				lreg = *((int*)(thread->bytecode + (++thread->programCounter)));
				memcpy(memory->stack + memory->stackPointer,
						thread->bytecode + 4 + thread->programCounter,
						lreg);
				thread->programCounter += (3+lreg);
				memory->stackPointer += lreg;
			break;
			
			//Decreases the stack pointer by 1
			case Pop1:
				memory->stackPointer--;
			break;
			
			//Decreases the stack pointer by 4
			case Pop4:
				//printf("pop4\n");
				memory->stackPointer-=4;
			break;
					
			//Decreases the stack pointer by 8
			case Pop8:
				memory->stackPointer-=sizeof(long);
			break;

			//Decreases the stack pointer by X (in bytecode)
			case PopX:
				memory->stackPointer-=
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
					memory->stack + (--memory->stackPointer),
					1
				);
				thread->programCounter += 3;
			break;

			//Saves 4 bytes from the stack to allocated space
			case Store4:
				//printf("store4\n");
				memory->stackPointer -= 4;
				memcpy(
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					memory->stack + (memory->stackPointer),
					4
				);
				thread->programCounter += 3;
			break;

			//Saves 8 bytes from the stack to allocated space
			case Store8:
				memory->stackPointer -= sizeof(long);
				memcpy(	
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					memory->stack + (memory->stackPointer),
					sizeof(long)
				);
				thread->programCounter += 3;		
			break;

			//Saves X bytes from stack to allocated space
			case StoreX:
				//printf("storex\n");
				//Data size
				ireg = *((int*)(thread->bytecode + (++thread->programCounter)))+1;
				thread->programCounter+=4;
				memory->stackPointer-=ireg;
				memcpy(
					memory->storage[
						(*(int*)(thread->bytecode +(thread->programCounter))) +
						altStoragePointer
					]->data,
					memory->stack + (++memory->stackPointer),
					ireg);
				thread->programCounter += 3;
			break;
	
			//Copies 1 byte of data from storage onto the stack
			case Load1:
				memcpy
				(
					memory->stack + (memory->stackPointer++),
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
					memory->stack + (memory->stackPointer),
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					4
				);
				memory->stackPointer += 4;
				thread->programCounter += 3;
			break;

			//Copies 8 bytes of data from storage onto the stack
			case Load8:
				memcpy
				(
					memory->stack + (memory->stackPointer),
					memory->storage[
						*(int*)(thread->bytecode +(++thread->programCounter)) +
						altStoragePointer
					]->data,
					sizeof(long)
				);
				memory->stackPointer += sizeof(long);
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
					memory->stack + (memory->stackPointer),
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
				if(ireg >= memory->storageSize)
				{
					int toClear = 0;
					int start = memory->storageSize;
					while(ireg >= toClear)
					{
						toClear += memory->reallocSize;
					}
					memory->storageSize += toClear;
					//Reallocate memory
					memory->storage = realloc(memory->storage,memory->storageSize);

					//clear memory
					memset(memory->storage + start, 0, toClear * sizeof(void*));
				}
				if(memory->storage[ireg] != NULL)
				{
					*storageCount-=1;
				}
				memory->storage[ireg] = malloc(sizeof(silentBlock));
				memory->storage[ireg]->data = malloc(1);
				silentSavePointer(gb,&memory->storage[ireg]);
				*storageCount+=1;
			break;

			//Allocates 4 bytes of data for the program
			case Alloc4:
				//printf("alloc4\n");
				ireg = *(int*)(thread->bytecode +(++thread->programCounter));
				thread->programCounter += 3;
				ireg += altStoragePointer;
				if(ireg >= localStoragePointer)
					localStoragePointer = ireg + 1;				
				if(ireg >= memory->storageSize)
				{
					int toClear = 0;
					int start = memory->storageSize;
					while(ireg >= toClear)
					{
						toClear += memory->reallocSize;
					}
					memory->storageSize += toClear;
					//Reallocate memory
					memory->storage = realloc(memory->storage,memory->storageSize);

					//clear memory
					memset(memory->storage + start, 0,toClear * sizeof(void*));
				}
				if(memory->storage[ireg] != NULL)
				{
					*storageCount-=1;
				}
				memory->storage[ireg] = malloc(sizeof(silentBlock));
				memory->storage[ireg]->data = malloc(4);
				silentSavePointer(gb,&memory->storage[ireg]);
				*storageCount+=1;
			break;

			//Allocates 8 bytes of data for the program
			case Alloc8:
				ireg = *(int*)(thread->bytecode +(++thread->programCounter));
				thread->programCounter += 3;
				ireg += altStoragePointer;
				if(ireg >= localStoragePointer)
					localStoragePointer = ireg + 1;
				if(ireg >= memory->storageSize)
				{
					int toClear = 0;
					int start = memory->storageSize;
					while(ireg >= toClear)
					{
						toClear += memory->reallocSize;
					}
					memory->storageSize += toClear;
					//Reallocate memory
					memory->storage = realloc(memory->storage,memory->storageSize);

					//clear memory
					memset(memory->storage + start, 0,toClear * sizeof(void*));
				}
				if(memory->storage[ireg] != NULL)
				{
					*storageCount-=1;
				}
				memory->storage[ireg] = malloc(sizeof(silentBlock));
				memory->storage[ireg]->data = malloc(sizeof(long));
				silentSavePointer(gb,&memory->storage[ireg]);
				*storageCount+=1;
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
				if(lreg >= memory->storageSize)
				{
					int toClear = 0;
					int start = memory->storageSize;
					while(lreg >= toClear)
					{
						toClear += memory->reallocSize;
					}
					memory->storageSize += toClear;
					//Reallocate memory
					memory->storage = realloc(memory->storage,memory->storageSize);

					//clear memory
					memset(memory->storage + start, 0,toClear * sizeof(void*));
				}
				if(memory->storage[ireg] != NULL)
				{
					*storageCount-=1;
				}
				memory->storage[lreg] = malloc(sizeof(silentBlock));
				memory->storage[lreg]->data = malloc(ireg);
				silentSavePointer(gb,&memory->storage[ireg]);
				*storageCount+=1;
			break;
			
			case GetPtr:
				//printf("getptr\n");
				thread->programCounter+=1;
				memcpy(memory->stack + memory->stackPointer,
						(int*)&memory->storage[
							*(int*)(thread->bytecode + (thread->programCounter)) +
							altStoragePointer
						]->data,
						sizeof(long));
				thread->programCounter += 3;
				memory->stackPointer += sizeof(long);
			break;


			case LoadPtr1:
				memory->stackPointer-=sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memcpy(memory->stack + memory->stackPointer,
					(long*)lreg,1);
				memory->stackPointer+=1;
			break;

			case LoadPtr4:
				memory->stackPointer-=sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memcpy(memory->stack + memory->stackPointer,
					(long*)lreg,4);
				memory->stackPointer+=4;
			break;

			case LoadPtr8:
				memory->stackPointer-=sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memcpy(memory->stack + memory->stackPointer,
					(long*)lreg,sizeof(long));
				memory->stackPointer+=sizeof(long);
			break;

			case LoadPtrX:
				memory->stackPointer-=sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				ireg = *(int*)(thread->bytecode + (++thread->programCounter));
				memcpy(memory->stack + memory->stackPointer,
					(long*)lreg,ireg);
				thread->programCounter += 3;
				memory->stackPointer += ireg;
			break;

			case EditPtr1:
				//printf("editptr1\n");
				memory->stackPointer -= sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memory->stackPointer -= 1;
				memcpy(
					(void*)lreg,
					(memory->stack + (memory->stackPointer)),
					sizeof(char)
				);
			break;

			case EditPtr4:
				memory->stackPointer -= sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memory->stackPointer -= 4;
				memcpy(
					(void*)lreg,
					memory->stack + (memory->stackPointer),
					sizeof(int)
				);
			break;

			case EditPtr8:
				memory->stackPointer -= sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				memory->stackPointer -= sizeof(long);
				memcpy(
					(void*)lreg,
					memory->stack + (memory->stackPointer),
					sizeof(long)
				);
			break;

			case EditPtrX:
				memory->stackPointer -= sizeof(long);
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

			case FREE://not implemented
				memory->stackPointer -= sizeof(long);
				lreg = *(long*)(memory->stack + (memory->stackPointer));
				silentDeletePointer(memory,storageCount,(void*)lreg);
			break;

			//Adds together 2 bytes on the stack
			case AddByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) += 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			//Adds together 2 integers on the stack
			case AddInt:
				//printf("addint\n");
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) += 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			//Adds together 2 longs on the stack
			case AddLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) += 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			//Adds together 2 floats on the stack
			case AddFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) += 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			//Adds together 2 doubles on the stack
			case AddDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) += 
					*(double*)(memory->stack + memory->stackPointer);

			break;

			//Subtracts the last number from the second last number on the stack
			case SubByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) -= 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubInt:
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) -= 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) -= 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) -= 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			//Subtracts the last number from the second last number on the stack
			case SubDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) -= 
					*(double*)(memory->stack + memory->stackPointer);

			break;
			
			//Multiplies 2 bytes together
			case MulByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) *= 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			//Multiplies 2 integers together
			case MulInt:
				//printf("mulint\n");
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) *= 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			//Multiplies 2 longs together
			case MulLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) *= 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			//Multiplies 2 floats together
			case MulFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) *= 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			//Multiplies 2 doubles together
			case MulDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) *= 
					*(double*)(memory->stack + memory->stackPointer);
			break;

			//Divides 2 bytes
			case DivByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) /= 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			//Divides 2 integers
			case DivInt:
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) /= 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			//Divides 2 longs
			case DivLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) /= 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			//Divides 2 floats
			case DivFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) /= 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			//Divides 2 doubles
			case DivDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) /= 
					*(double*)(memory->stack + memory->stackPointer);
			break;

			//Byte to integer conversion
			case ByteToInt:
				memset(memory->stack + memory->stackPointer, 0, 3);
				memory->stackPointer += 3;
			break;

			//Byte to long conversion
			case ByteToLong:
				memset(memory->stack + memory->stackPointer, 0, sizeof(long)-1);
				memory->stackPointer += sizeof(long)-1;
			break;

			//Byte to float conversion
			case ByteToFloat:
				memory->stackPointer -= 1;
				breg =  *(char*)(memory->stack + (memory->stackPointer));
				freg = breg;
				memcpy(memory->stack + memory->stackPointer, &freg, 4);
				memory->stackPointer += 4;
			break;

			//Byte to double conversion
			case ByteToDouble://untested
				memory->stackPointer -= 1;
				breg =  *(char*)(memory->stack + (memory->stackPointer));
				dreg = breg;
				memcpy(memory->stack + memory->stackPointer, &dreg, sizeof(double));
				memory->stackPointer += sizeof(double);
			break;

			//Integer to byte conversion
			case IntToByte:
				memory->stackPointer-=3;
			break;

			//Integer to long conversion
			case IntToLong:
				memset(memory->stack + memory->stackPointer,0,sizeof(long)-sizeof(int));
				memory->stackPointer+=sizeof(long)-sizeof(int);
			break;
			
			//Integer to float conversion
			case IntToFloat:
				memory->stackPointer-=4;
				ireg =  *(int*)(memory->stack + (memory->stackPointer));
				freg = (float)ireg;
				memcpy(
					memory->stack + (memory->stackPointer),
					&freg,
					4);
				memory->stackPointer+=4;
			break;

			//Integer to double conversion
			case IntToDouble:
				memory->stackPointer-=4;
				ireg =  *(int*)(memory->stack + (memory->stackPointer));
				dreg = (double)ireg;
				memcpy(
					memory->stack + memory->stackPointer,
					&dreg,
					sizeof(long));
				memory->stackPointer+=sizeof(double)-sizeof(int);
			break;


			case FloatToByte:
				memory->stackPointer-=4;
				freg =  *(float*)(memory->stack + (memory->stackPointer));
				breg = (char)freg;
				memcpy(memory->stack + (memory->stackPointer), &breg, 1);
				memory->stackPointer+=1;
			break;

			//Float to integer conversion
			case FloatToInt:
				memory->stackPointer-=4;
				freg =  *(float*)(memory->stack + (memory->stackPointer));
				ireg = (int)freg;
				memcpy(memory->stack + (memory->stackPointer), &ireg, 4);
				memory->stackPointer+=4;
			break;

			case FloatToLong:
				memory->stackPointer-=4;
				freg =  *(float*)(memory->stack + (memory->stackPointer));
				lreg = (long)freg;
				memcpy(memory->stack + (memory->stackPointer), &lreg, sizeof(long));
				memory->stackPointer+=sizeof(long);
			break;

			//Float to double conversion
			case FloatToDouble:
				memory->stackPointer-=4;
				freg =  *(float*)(memory->stack + (memory->stackPointer));
				dreg = (double)freg;
				memcpy(memory->stack + (memory->stackPointer), &dreg, sizeof(double));
				memory->stackPointer+=sizeof(double);			
			break;

			case DoubleToByte:
				memory->stackPointer-=sizeof(long);
				dreg =  *(double*)(memory->stack + (memory->stackPointer));
				breg = (char)dreg;
				memcpy(memory->stack + (memory->stackPointer), &breg, 1);
				memory->stackPointer+=1;
			break;

			case DoubleToInt:
				memory->stackPointer-=sizeof(long);
				dreg =  *(double*)(memory->stack + (memory->stackPointer));
				ireg = (int)dreg;
				memcpy(memory->stack + (memory->stackPointer), &ireg, 4);
				memory->stackPointer+=4;
			break;

			case DoubleToLong:
				memory->stackPointer-=sizeof(double);
				dreg =  *(double*)(memory->stack + (memory->stackPointer));
				lreg = (long)dreg;
				memcpy(memory->stack + (memory->stackPointer), &lreg, sizeof(long));
				memory->stackPointer+=sizeof(long);
			break;

			case DoubleToFloat:
				memory->stackPointer-=sizeof(double);
				dreg =  *(double*)(memory->stack + (memory->stackPointer));
				freg = (float)dreg;
				memcpy(memory->stack + (memory->stackPointer), &freg, 4);
				memory->stackPointer+=4;
			break;

			case LongToByte:
				memory->stackPointer -= sizeof(long)-1;
			break;

			case LongToInt:
				memory->stackPointer -= sizeof(long)-sizeof(int);
			break;

			case LongToFloat:
				memory->stackPointer-=sizeof(long);
				ireg =  *(long*)(memory->stack + (memory->stackPointer));
				freg = (float)ireg;
				memcpy(
					memory->stack + (memory->stackPointer),
					&freg,
					4);
				memory->stackPointer+=4;
			break;

			case LongToDouble:
				memory->stackPointer-=sizeof(long);
				ireg =  *(long*)(memory->stack + (memory->stackPointer));
				dreg = (double)ireg;
				memcpy(
					memory->stack + (memory->stackPointer),
					&dreg,
					sizeof(double));
				memory->stackPointer+=sizeof(double);
			break;
			
			//Compare value of 2 bytes
			case SmallerThanByte:
				memory->stackPointer--;
				if(*(char*)(memory->stack + (memory->stackPointer-1)) <
					*(char*)(memory->stack + memory->stackPointer))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case SmallerThanInt:
				//printf("smallerint\n");
				memory->stackPointer-=7;
				if(*(int*)(memory->stack + memory->stackPointer-1) < 
					*(int*)(memory->stack + memory->stackPointer+3))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}			
			break;

			//Compare value of 2 8 bytes
			case SmallerThanLong:
				//memory->stackPointer-=15;
				memory->stackPointer-=(sizeof(long)*2)-1;
				if(*(long*)(memory->stack + (memory->stackPointer-1)) < 
					*(long*)(memory->stack + memory->stackPointer+(sizeof(long)-1)))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case SmallerThanFloat:
				memory->stackPointer-=7;
				if(*(float*)(memory->stack + (memory->stackPointer-1)) < 
					*(float*)(memory->stack + memory->stackPointer+3))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case SmallerThanDouble:
				memory->stackPointer-=(sizeof(double)*2)-1;
				if(*(double*)(memory->stack + (memory->stackPointer-1)) < 
					*(double*)(memory->stack + memory->stackPointer+(sizeof(double)-1)))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 bytes
			case BiggerThanByte:
				memory->stackPointer--;
				if(*(char*)(memory->stack + (memory->stackPointer-1)) > 
					*(char*)(memory->stack + memory->stackPointer))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case BiggerThanInt:
				memory->stackPointer-=7;
				if(*(int*)(memory->stack + (memory->stackPointer-1)) > 
					*(int*)(memory->stack + memory->stackPointer+3))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case BiggerThanLong:
				memory->stackPointer-=(sizeof(long)*2)-1;
				if(*(long*)(memory->stack + (memory->stackPointer-1)) > 
					*(long*)(memory->stack + memory->stackPointer+(sizeof(long)-1)))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case BiggerThanFloat:
				memory->stackPointer-=7;
				if(*(float*)(memory->stack + (memory->stackPointer-1)) > 
					*(float*)(memory->stack + memory->stackPointer+3))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case BiggerThanDouble:
				memory->stackPointer-=(sizeof(double)*2)-1;
				if(*(double*)(memory->stack + (memory->stackPointer-1)) > 
					*(double*)(memory->stack + memory->stackPointer+(sizeof(double)-1)))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 bytes
			case EqualByte:
				memory->stackPointer--;
				if(*(char*)(memory->stack + (memory->stackPointer-1)) == 
					*(char*)(memory->stack + memory->stackPointer))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case EqualInt:
				memory->stackPointer-=7;
				if(*(int*)(memory->stack + (memory->stackPointer-1)) == 
					*(int*)(memory->stack + memory->stackPointer+3))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case EqualLong:
				memory->stackPointer-=(sizeof(long)*2)-1;
				if(*(long*)(memory->stack + (memory->stackPointer-1)) == 
					*(long*)(memory->stack + memory->stackPointer+(sizeof(long)-1)))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 4 bytes
			case EqualFloat:
				memory->stackPointer-=7;
				if((*(float*)(memory->stack + (memory->stackPointer-1))) == 
					(*(float*)(memory->stack + memory->stackPointer+3)))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;

			//Compare value of 2 8 bytes
			case EqualDouble:
				memory->stackPointer-=15;
				if(*(double*)(memory->stack + (memory->stackPointer-1)) ==
					*(double*)(memory->stack + memory->stackPointer+7))
				{
					memory->stack[memory->stackPointer-1] = 1;
				}
				else
				{
					memory->stack[memory->stackPointer-1] = 0;
				}
			break;


			case If:
				//printf("if\n");
				if(*(char*)(memory->stack + --memory->stackPointer))
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
				if(!(*(char*)(memory->stack + (--memory->stackPointer))))
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
		thread->programCounter++;
	}
}
