#include "SilentVM2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEBUG 0
#define STACK_OUTPUT 0

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

typedef enum dataType
{
	INT8 = 0,
	UINT8,
	INT16,
	UINT16,
	INT32,
	UINT32,
	INT64,
	UINT64,
	FLOAT32,
	FLOAT64,
	POINTER,
	POINTER_LOCATION,
	UNDEFINED,
	UNDEFINED_END
}dataType;

SilentMemory* createSilentMemory(
	uint stackBufferSize, uint heapBufferSize
)
{
	SilentMemory* memory 	= malloc(sizeof(SilentMemory));
	memory->stack 			= malloc(stackBufferSize);
	memory->stackSize		= stackBufferSize;

	//memory->heap			= SilentCreateVector(heapBufferSize, 8);
	//memory->stackTypes		= SilentCreateVector(stackBufferSize/4, 1);
	//memory->stackFrame		= SilentCreateVector(stackBufferSize/4, 8);
	return memory; 
}

SilentVM* createSilentVM(SilentMemory* memory, char* program, SilentGC* gc)
{
	SilentVM* vm 		= malloc(sizeof(SilentVM));
	vm->memory 			= memory;
	vm->gc 				= gc;
	vm->dlls 			= malloc(sizeof(SilentDll));
	vm->dllCount		= 0;
	vm->program 		= program;
	vm->running 		= 0;
	return vm;
}

SilentGC* createSilentGC(SilentMemory* memory)
{
	SilentGC* gc = malloc(sizeof(SilentGC));
	gc->memory = memory;
	return gc;
}

//Delete program's allocated space
void deleteSilentMemory(SilentMemory* memory)
{
	free(memory->stack);
	SilentDeleteVector(memory->stackTypes);
	SilentDeleteVector(memory->heap);
	SilentDeleteVector(memory->stackFrame);
	free(memory);
}

void deleteSilentVM(SilentVM* vm)
{
	if(vm->memory != NULL) deleteSilentMemory(vm->memory);
	free(vm);
}

void deleteSilentGC(SilentGC* gc)
{
	free(gc);
}

char SilentGetTypeSize(char type)
{
	switch(type)
	{
		case INT8:
		case UINT8:
			return 1;
		break;
		
		case INT16:
		case UINT16:
			return 2;
		break;

		case INT32:
		case UINT32:
		case FLOAT32:
			return 4;
		break;

		case INT64:
		case UINT64:
		case FLOAT64:
		case POINTER:
		case POINTER_LOCATION:
			return 8;
		break;

		case UNDEFINED:
		case UNDEFINED_END:
			return 0;
		break;

		default:
			return -1;
		break;
	}
}

void silentVMStart(SilentVM* vm)
{
	union Registers
	{
		char c;
		short s;
		int i;
		long long l;
		float f;
		double d;
	}reg, reg2, reg3, reg4;

	dataType dt[] = {
		INT8,
		UINT8,
		INT16,
		UINT16,
		INT32,
		UINT32,
		INT64,
		UINT64,
		FLOAT32,
		FLOAT64,
		POINTER,
		POINTER_LOCATION,
		UNDEFINED,
		UNDEFINED_END
	};


	char* 				program = vm->program;
	char* 				stack 	= vm->memory->stack;
	//SilentVector*		heap	= vm->memory->heap;
	//SilentVector* 		stackT  = vm->memory->stackTypes;
	char* stackT = malloc(8000);
	uint64 stackTPtr = 0;

	//SilentVector*		saveSFData = SilentCreateVector(32000, 8);
	unsigned long long* saveSFData = malloc(80000);
	unsigned long long  saveSFDataPtr = 0;

	SilentDll*			dlls = vm->dlls;

	
	uint64 sp = 0; //stack pointer
	uint64 fp = 0; //frame pointer
	//uint64 lp = 0; //local pointer
	uint64 pc = 0; //program counter

	
	vm->running = 1;
	char* tempPtr;

	char running = 1;
	while(running)
	{
		switch(program[pc])
		{
			case Halt:
				#if DEBUG
					printf("halt\n");
				#endif
				running = 0;
				vm->running = 0;
			break;
			
			case Goto:
				pc++;
				pc = *((uint64*)(program + (pc)));
				#if DEBUG
					printf("Goto\n");
					printf("pc: %i\n",pc);
				#endif
				pc--;
			break;

			case Sweep:
				SilentMark(vm->gc);
				SilentSweep(vm->gc);
			break;

			case Call:
				pc++;
				//Get pointer to the subroutine
				memcpy(&reg.l, program + pc, 8);
				pc += 8;
				//Get number of arguments
				memcpy(&reg2.l, program + pc, 8);
				pc += 8;
				//Get total size of arguments
				memcpy(&reg3.l, program + pc, 8);
				pc += 7;

				//Save old stackframe data
				//SilentPushBack(saveSFData, &fp);
				saveSFData[saveSFDataPtr++] = fp;
				//Push arguments to new subroutine
				fp = sp;
				fp -= reg3.l;

				

				//stackT->ptr -= reg2.l;
				//SilentPushBack(saveSFData, &stackT->ptr);
				//stackT->ptr += reg2.l;
				stackTPtr -= reg2.l;
				saveSFData[saveSFDataPtr++] = stackTPtr;
				stackTPtr += reg2.l;

				sp -= reg3.l;
				saveSFData[saveSFDataPtr++] = sp;
				//SilentPushBack(saveSFData, &sp);
				sp += reg3.l;

				//Save return position
				//SilentPushBack(saveSFData, &pc);

				//Go to subroutine
				pc = reg.l - 1;

				#if DEBUG
					printf("Call\n");
					printf("pc: %i\n",pc);
					printf("fp: %i\n",fp);
					printf("arg size: %i\n", reg2.l);
				#endif
			break;

			case Return:
				pc++;
				//Number of return values
				//Get number of return values
				memcpy(&reg.l, program + pc, 8);
				pc += 8;
				//Get size of return values
				memcpy(&reg2.l, program + pc, 8);
				//pc += 7;

				//Go back to old subroutine
				//SilentPopBack(saveSFData);
				//memcpy(&pc, saveSFData->data + saveSFData->ptr, 8);
				pc = saveSFData[--saveSFDataPtr];

				//Get old stack pointer
				//SilentPopBack(saveSFData);
				reg3.l = sp;
				//memcpy(&sp, saveSFData->data + saveSFData->ptr, 8);
				sp = saveSFData[--saveSFDataPtr];

				//Move return data to stack pointer
				memcpy(stack + sp, stack + reg3.l - reg2.l, reg2.l);
				sp += reg2.l; //Add returned values to stack pointer

				//Get old stack types
				//SilentPopBack(saveSFData);
				stackTPtr--;

				//Get old stack types ptr
				//reg3.l = stackT->ptr;
				reg3.l = stackTPtr;
				//memcpy(&stackT->ptr, saveSFData->data + saveSFData->ptr, 8);
				stackTPtr = saveSFData[saveSFDataPtr];
				//stackT->ptr += reg.l; //Add returned values to the pointer
				saveSFDataPtr += reg.l;

				//Move stack types to old pointer
				//memcpy(stackT->data + stackT->ptr,
				//	stackT->data + reg3.l - reg.l, reg.l);
				memcpy(stackT + stackTPtr, stackT + reg3.l - reg.l, reg.l);

				//Get old frame pointer
				//SilentPopBack(saveSFData);
				//memcpy(&fp, saveSFData->data + saveSFData->ptr, 8);
				fp = saveSFData[--saveSFDataPtr];

				#if DEBUG
					printf("Return\n");
					printf("pc: %i\n",pc);
					printf("fp: %i\n",fp);
				#endif
			break;

			case LoadDll:
				//Get dll name
				pc++;
				memcpy(&reg.l, program + pc, 8);
				pc += 8;
				tempPtr = program + pc;
				pc += reg.l-1;
				//Create dll
				dlls[vm->dllCount].procs = malloc(sizeof(SilentDllProc));
				dlls[vm->dllCount].name = malloc(reg.l);
				memcpy(dlls[vm->dllCount].name, tempPtr, reg.l);
				dlls[vm->dllCount].addr = SilentLoadLibrary(tempPtr);
				dlls[vm->dllCount].count = 0;
				vm->dllCount += 1;
				do
				{
					tempPtr = realloc(dlls,(vm->dllCount+1)*sizeof(SilentDll));
				}while(tempPtr == NULL);
				dlls = (SilentDll*)tempPtr;
				#if DEBUG
				printf("LoadDll\n");
				printf("addr %i\n",dlls[0].addr);
				#endif
			break;

			case LoadDllFunc:
				//Get dll index
				pc++;
				memcpy(&reg.l, program + pc, 8);
				pc += 8;
				SilentDll* dll = dlls + reg.l;
				//Get proc name
				memcpy(&reg2.l, program + pc, 8);
				pc += 8;
				dll->procs[dll->count].procName = malloc(reg2.l);
				memcpy(dll->procs[dll->count].procName, program+pc, reg2.l);
				pc += reg2.l-1;
				
				dll->procs[dll->count].addr = 
				SilentLoadFunc(dll->addr, dll->procs[dll->count].procName);
				dll->count += 1;
				do
				{
					tempPtr = realloc(dll->procs,(dll->count+1)*sizeof(SilentDllProc));
				}while(tempPtr == NULL);
				dll->procs = (SilentDllProc*)tempPtr;
				#if DEBUG
				printf("Load dll func\n");
				printf("addr %i\n",dlls[0].addr);
				#endif
			break;

			case FreeDll:
				pc++;
				memcpy(&reg.l, program + pc, 8);
				pc += 7;
				SilentFreeLibrary(dlls[reg.l].addr);
			break;

			case CallDllFunc:
				pc++;
				//Dll index
				memcpy(&reg.l, program + pc, 8);
				pc += 8;
				//Function index
				memcpy(&reg2.l, program + pc, 8);
				pc += 8;
				//number of arguments index
				memcpy(&reg3.l, program + pc, 8);
				pc += 8;

				uint64 argSize = 0;
				for(uint64 i = 0; i < reg3.l; i++)
				{
					//reg4.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
					reg4.c = SilentGetTypeSize(stackT[stackTPtr--]);
					//if(reg4.c != 0)
					//{
						argSize += reg4.c;
					//	SilentPopBack(stackT);
					//}
					//else
					//{
						//uint64 dSize;
						//memcpy(&dSize, stackT->data + stackT->ptr - 9, 8);
						//argSize += dSize;
					//	argSize += stackT[stackTPtr - 9];
					//	SilentPopMultiple(stackT,10);
					//}
				}
				reg3.l = argSize;
				sp -= argSize;

				vm->memory->framePointer = fp;
				vm->memory->stackPointer = sp;
				vm->programCounter = pc;
				dlls[reg.l].procs[reg2.l].addr(vm);

				fp = vm->memory->framePointer;
				sp = vm->memory->stackPointer;
				pc = vm->programCounter-1;
			break;

			case Push1:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				stack[sp++] = program[++pc];
			break;

			case Push2:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(stack + sp, program + (++pc), 2);
				sp += 2;
				pc++;
			break;

			case Push4:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(stack + sp, program + (++pc), 4);
				sp += 4;
				pc += 3;
			break;

			case Push8:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(stack + sp, program + (++pc), 8);
				sp += 8;
				pc += 7;
			break;

			case Pop1:
				//SilentPopBack(stackT);
				stackTPtr--;
				sp -= 1;
			break;

			case Pop2:
				//SilentPopBack(stackT);
				stackTPtr--;
				sp -= 2;
			break;

			case Pop4:
				//SilentPopBack(stackT);
				stackTPtr--;
				sp -= 4;
			break;

			case Pop8:
				//SilentPopBack(stackT);
				stackTPtr--;
				sp -= 8;
			break;

			case Store1:
				//SilentPopBack(stackT);
				stackTPtr--;
				memcpy(&reg2.l, program + (++pc), 8);
				pc += 7;
				memcpy(stack + fp + reg2.l, stack + --sp, 1);
			break;

			case Store2:
				//SilentPopBack(stackT);
				stackTPtr--;
				memcpy(&reg2.l, program + (++pc), 8);
				pc += 7;
				sp -= 2;
				memcpy(stack + fp + reg2.l, stack + sp, 2);
			break;

			case Store4:
				//SilentPopBack(stackT);
				stackTPtr--;
				memcpy(&reg2.l, program + (++pc), 8);
				pc += 7;
				sp -= 4;
				memcpy(stack + fp + reg2.l, stack + sp, 4);
			break;

			case Store8:
				//SilentPopBack(stackT);
				stackTPtr--;
				memcpy(&reg2.l, program + (++pc), 8);
				pc += 7;
				sp -= 8;
				memcpy(stack + fp + reg2.l, stack + sp, 8);
			break;

			case Load1:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 1);
				sp++;
			break;

			case Load2:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 2);
				sp += 2;
			break;

			case Load4:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 4);
				sp += 4;
			break;

			case Load8:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 9);
				sp += 8;
			break;

			case StoreGlobal1:
				//Get global offset
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				//Store
				sp -= 1;
				memcpy(stack + reg.l, stack + sp, 1);
				//SilentPopBack(stackT);
				stackTPtr--;
			break;
			
			case StoreGlobal2:
				//Get global offset
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				//Store
				sp -= 2;
				memcpy(stack + reg.l, stack + sp, 2);
				//SilentPopBack(stackT);
				stackTPtr--;
			break;

			case StoreGlobal4:
				//Get global offset
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				//Store
				sp -= 4;
				memcpy(stack + reg.l, stack + sp, 4);
				//SilentPopBack(stackT);
				stackTPtr--;
			break;

			case StoreGlobal8:
				//Get global offset
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				//Store
				sp -= 8;
				memcpy(stack + reg.l, stack + sp, 8);
				//SilentPopBack(stackT);
				stackTPtr--;
			break;

			case LoadGlobal1:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 1);
				sp++;
			break;

			case LoadGlobal2:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 2);
				sp += 2;
			break;

			case LoadGlobal4:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 4);
				sp += 4;
			break;

			case LoadGlobal8:
				//SilentPushBack(stackT, dt + program[++pc]);
				stackT[stackTPtr++] = program[++pc];
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 8);
				sp += 8;
			break;

			case Alloc:
				//get alloc size
				memcpy(&reg.l, program + ++pc, 8);
				pc += 7;
				//allocate space
				reg2.l = SilentAlloc(vm->gc, reg.l);
				//push pointer location
				memcpy(stack + sp, &reg2.l, 8);
				sp += 8;
				//SilentPushBack(stackT, dt + POINTER_LOCATION);
				stackT[stackTPtr++] = POINTER_LOCATION;
			break;

			case LoadPtr:
				//get pointer
				sp -= 8;
				memcpy(&tempPtr, stack + sp, 8);
				//SilentPopBack(stackT);
				stackTPtr--;
				switch(program[++pc])
				{
					case INT8:
						memcpy(stack + sp, tempPtr, 1);
						sp += 1;
						//SilentPushBack(stackT,dt + INT8);
						stackT[stackTPtr++] = INT8;
					break;
					case UINT8:
						memcpy(stack + sp, tempPtr, 1);
						sp += 1;
						//SilentPushBack(stackT,dt + UINT8);
						stackT[stackTPtr++] = UINT8;
					break;
					case INT16:
						memcpy(stack + sp, tempPtr, 2);
						sp += 2;
						//SilentPushBack(stackT,dt + INT16);
						stackT[stackTPtr++] = INT16;
					break;
					case UINT16:
						memcpy(stack + sp, tempPtr, 2);
						sp += 2;
						//SilentPushBack(stackT,dt + UINT16);
						stackT[stackTPtr++] = UINT16;
					break;
					case INT32:
						memcpy(stack + sp, tempPtr, 4);
						sp += 4;
						//SilentPushBack(stackT,dt + INT32);
						stackT[stackTPtr++] = INT32;
					break;
					case UINT32:
						memcpy(stack + sp, tempPtr, 4);
						sp += 4;
						//SilentPushBack(stackT,dt + UINT32);
						stackT[stackTPtr++] = UINT32;
					break;
					case INT64:
						memcpy(stack + sp, tempPtr, 8);
						sp += 8;
						//SilentPushBack(stackT,dt + INT64);
						stackT[stackTPtr++] = INT64;
					break;
					case UINT64:
						memcpy(stack + sp, tempPtr, 8);
						sp += 8;
						//SilentPushBack(stackT,dt + UINT64);
						stackT[stackTPtr++] = UINT64;
					break;
					case FLOAT32:
						memcpy(stack + sp, tempPtr, 4);
						sp += 4;
						//SilentPushBack(stackT,dt + FLOAT32);
						stackT[stackTPtr++] = FLOAT32;
					break;
					case FLOAT64:
						memcpy(stack + sp, tempPtr, 8);
						sp += 8;
						//SilentPushBack(stackT,dt + FLOAT64);
						stackT[stackTPtr++] = FLOAT64;
					break;
					case POINTER:
						memcpy(stack + sp, tempPtr, 8);
						sp += 8;
						//SilentPushBack(stackT,dt + POINTER);
						stackT[stackTPtr++] = POINTER;
					break;
					case POINTER_LOCATION:
						memcpy(stack + sp, tempPtr, 8);
						sp += 8;
						//SilentPushBack(stackT,dt + POINTER_LOCATION);
						stackT[stackTPtr++] = POINTER_LOCATION;
					break;
					default: break;
				}
			break;

			case StorePtr:
				//Get pointer
				sp -= 8;
				memcpy(&tempPtr, stack + sp, 8);
				//SilentPopBack(stackT);
				//reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
				reg.c = SilentGetTypeSize(stackT[--stackTPtr]);
				//if(reg.c != 0)
				//{
					sp -= reg.c;
					memcpy(tempPtr, stack + sp, reg.c);
					//SilentPopBack(stackT);
				//}
			break;

			case GetPtr:
				//sp -= 8;
				//memcpy(&reg.l, stack + sp, 8);
				//SilentPopBack(stackT);
				//tempPtr = ((SilentMemoryBlock*)heap->data)[reg.l].data;
				//memcpy(stack + sp, &tempPtr, 8);
				//sp += 8;
				//SilentPushBack(stackT, dt + POINTER);
			break;

			case FreePtr:
				sp -= 8;
				memcpy(&tempPtr, stack + sp, 8);
				free(tempPtr);
				//SilentPopBack(stackT);
				stackTPtr--;
			break;

			case Free:
				sp -= 8;
				memcpy(&reg.l, stack + sp, 8);
				//SilentPopBack(stackT);
				stackTPtr--;
				SilentFree(vm->gc, reg.l);
			break;

			case Add:
				//SilentPopBack(stackT);
				//switch(stackT->data[stackT->ptr-1])
				stackTPtr--;
				switch(stackT[stackTPtr-1])
				{
					case INT8:
					case UINT8:
						sp--;
						//stack[sp] += program[pc+1];
						(*(unsigned char*)(stack + sp-1)) +=
						(*(unsigned char*)(stack + sp));
					break;
					case INT16:
					case UINT16:
						sp -= 2;
						(*(unsigned short*)(stack + sp-2)) +=
						(*(unsigned short*)(stack + sp));
					break;
					case INT32:
					case UINT32:
						sp -= 4;
						(*(unsigned int*)(stack + sp-4)) +=
						(*(unsigned int*)(stack + sp));
					break;
					case INT64:
					case UINT64:
					case POINTER:
						sp -= 8;
						(*(uint64*)(stack + sp-8)) +=
						(*(uint64*)(stack + sp));
					break;
					case FLOAT32:
						sp -= 4;
						(*(float*)(stack + sp-4)) +=
						(*(float*)(stack + sp));
					break;
					case FLOAT64:
						sp -= 8;
						(*(double*)(stack + sp-8)) +=
						(*(double*)(stack + sp));
					break;
					default: break;
				}
				#if DEBUG
					printf("Add\n");
				#endif
			break;

			case Sub:
				//SilentPopBack(stackT);
				//switch(stackT->data[stackT->ptr-1])
				stackTPtr--;
				switch(stackT[stackTPtr-1])
				{
					case INT8:
					case UINT8:
						sp--;
						(*(unsigned short*)(stack + sp-1)) -=
						(*(unsigned short*)(stack + sp));
					break;
					case INT16:
					case UINT16:
						sp -= 2;
						(*(unsigned short*)(stack + sp-2)) -=
						(*(unsigned short*)(stack + sp));
					break;
					case INT32:
					case UINT32:
						sp -= 4;
						(*(unsigned int*)(stack + sp-4)) -=
						(*(unsigned int*)(stack + sp));
					break;
					case INT64:
					case UINT64:
					case POINTER:
						sp -= 8;
						(*(uint64*)(stack + sp-8)) -=
						(*(uint64*)(stack + sp));
					break;
					case FLOAT32:
						sp -= 4;
						(*(float*)(stack + sp-4)) -=
						(*(float*)(stack + sp));
					break;
					case FLOAT64:
						sp -= 8;
						(*(double*)(stack + sp-8)) -=
						(*(double*)(stack + sp));
					break;
					default: break;
				}
				#if DEBUG
					printf("Sub\n");
				#endif
			break;

			case Mul:
				//SilentPopBack(stackT);
				//switch(stackT->data[stackT->ptr-1])
				stackTPtr--;
				switch(stackT[stackTPtr-1])
				{
					case INT8:
					case UINT8:
						sp--;
						(*(unsigned short*)(stack + sp-1)) *=
						(*(unsigned short*)(stack + sp));
					break;
					case INT16:
					case UINT16:
						sp -= 2;
						(*(unsigned short*)(stack + sp-2)) *=
						(*(unsigned short*)(stack + sp));
					break;
					case INT32:
					case UINT32:
						sp -= 4;
						(*(unsigned int*)(stack + sp-4)) *=
						(*(unsigned int*)(stack + sp));
					break;
					case INT64:
					case UINT64:
					case POINTER:
						sp -= 8;
						(*(uint64*)(stack + sp-8)) *=
						(*(uint64*)(stack + sp));
					break;
					case FLOAT32:
						sp -= 4;
						(*(float*)(stack + sp-4)) *=
						(*(float*)(stack + sp));
					break;
					case FLOAT64:
						sp -= 8;
						(*(double*)(stack + sp-8)) *=
						(*(double*)(stack + sp));
					break;
					default: break;
				}
				#if DEBUG
					printf("Mul\n");
				#endif
			break;

			case Div:
				//SilentPopBack(stackT);
				//switch(stackT->data[stackT->ptr-1])
				stackTPtr--;
				switch(stackT[stackTPtr-1])
				{
					case INT8:
						sp--;
						(*(char*)(stack + sp-1)) /=
						(*(char*)(stack + sp));
					break;
					case UINT8:
						sp--;
						(*(unsigned char*)(stack + sp-1)) /=
						(*(unsigned char*)(stack + sp));
					break;
					case INT16:
						sp -= 2;
						(*(short*)(stack + sp-2)) /=
						(*(short*)(stack + sp));
					break;
					case UINT16:
						sp -= 2;
						(*(unsigned short*)(stack + sp-2)) /=
						(*(unsigned short*)(stack + sp));
					break;
					case INT32:
						sp -= 4;
						(*(int*)(stack + sp-4)) /=
						(*(int*)(stack + sp));
					break;
					case UINT32:
						sp -= 4;
						(*(unsigned int*)(stack + sp-4)) /=
						(*(unsigned int*)(stack + sp));
					break;
					case INT64:
						sp -= 8;
						(*(int64*)(stack + sp-8)) /=
						(*(int64*)(stack + sp));
					break;
					case UINT64:
					case POINTER:
						sp -= 8;
						(*(uint64*)(stack + sp-8)) /=
						(*(uint64*)(stack + sp));
					break;
					case FLOAT32:
						sp -= 4;
						(*(float*)(stack + sp-4)) /=
						(*(float*)(stack + sp));
					break;
					case FLOAT64:
						sp -= 8;
						(*(double*)(stack + sp-8)) /=
						(*(double*)(stack + sp));
					break;
					default: break;
				}
				#if DEBUG
					printf("Div\n");
				#endif
			break;

			case Convert:
				//SilentPopBack(stackT);
				// reg2.l = 0;
				//switch(stackT->data[stackT->ptr])
				stackTPtr--;
				switch(stackT[stackTPtr])
				{
					case INT8:
						switch(program[pc++])
						{
							case INT8: stackT[stackTPtr++] = INT8; break;
							case UINT8:
								//SilentPushBack(stackT, dt + UINT8);
								stackT[stackTPtr++] = UINT8;
							break;
							
							case INT16:
								stack[sp++] = 0;
								//SilentPushBack(stackT, dt + INT16);
								stackT[stackTPtr++] = UINT8;
							break;

							case UINT16:
								stack[sp++] = 0;
								//SilentPushBack(stackT, dt + INT16);
								stackT[stackTPtr++] = UINT8;
							break;
							case INT32:
								memset(stack + sp, 0, 3);
								sp += 3;
						}
					break;
					case UINT8:
						sp--;
						reg.c = (unsigned char)stack[sp];
					break;
					case INT16:
						sp -= 2;
						reg.s = (short)*((short*)(stack+sp));
					break;
					case UINT16:
						sp -= 2;
						reg.s = (unsigned short)*((unsigned short*)(stack+sp));
					break;
					case INT32:
						sp -= 4;
						reg.i = (int)*((int*)(stack+sp));
					break;
					case UINT32:
						sp -= 4;
						reg.i = (unsigned int)*((unsigned int*)(stack+sp));
					break;
					case INT64:
						sp -= 8;
						reg.l = (long long)*((long long*)(stack+sp));
					break;
					case UINT64:
					case POINTER:
						sp -= 8;
						reg.l = (uint64)*((uint64*)(stack+sp));
					break;
					case FLOAT32:
						sp -= 4;
						reg.f = (float)*((float*)(stack+sp));
					break;
					case FLOAT64:
						sp -= 8;
						reg.d = (long double)*((long double*)(stack+sp));
					break; 
				}

				switch(program[++pc])
				{
					case INT8:
						reg2.c = (char)reg2.l;
						stack[sp] = reg2.c;
						sp += 1;
						//SilentPushBack(stackT, dt + INT8);
					break;
					case UINT8:
						reg3.c = (unsigned char)stack[sp];
						stack[sp] = reg3.c;
						sp += 1;
						//SilentPushBack(stackT, dt + UINT8);
					break;
					case INT16:
						reg.s = (short)*((short*)(stack+sp));
						memcpy(stack + sp, &reg.s, 2);
						sp += 2;
						//SilentPushBack(stackT, dt + INT16);
					break;
					case UINT16:
						reg.s = (unsigned short)*((unsigned short*)(stack+sp));
						memcpy(stack + sp, &reg.s, 2);
						sp += 2;
						//SilentPushBack(stackT, dt + INT16);
					break;
					case INT32:
						
					break;
					case UINT32:
						
					break;
					case INT64:
						
					break;
					case UINT64:
					case POINTER:
						
					break;
					case FLOAT32:
						
					break;
					case FLOAT64:
						
					break;
				}
			break;

			case SmallerThan:
				//SilentPopBack(stackT);
				//SilentPopBack(stackT);
				//switch(stackT->data[stackT->ptr])
				stackTPtr--;
				switch(stackT[--stackTPtr])
				{
					case INT8:
						sp-=2;
						reg.c = (*(char*)(stack + sp)) <
						(*(char*)(stack + sp+1));
					break;
					case UINT8:
						sp-=2;
						reg.c = (*(unsigned char*)(stack + sp)) <
						(*(unsigned char*)(stack + sp+1));
					break;
					case INT16:
						sp -= 4;
						reg.c = (*(short*)(stack + sp)) <
						(*(short*)(stack + sp+2));
					break;
					case UINT16:
						sp -= 4;
						reg.c = (*(unsigned short*)(stack + sp)) <
						(*(unsigned short*)(stack + sp+2));
					break;
					case INT32:
						sp -= 8;
						reg.c = (*(int*)(stack + sp)) <
						(*(int*)(stack + sp+4));
					break;
					case UINT32:
						sp -= 8;
						reg.c = (*(unsigned int*)(stack + sp)) <
						(*(unsigned int*)(stack + sp+4));
					break;
					case INT64:
						sp -= 16;
						reg.c = (*(int64*)(stack + sp)) <
						(*(int64*)(stack + sp+8));
					break;
					case UINT64:
					case POINTER:
						sp -= 16;
						reg.c = (*(uint64*)(stack + sp)) <
						(*(uint64*)(stack + sp+8));
					break;
					case FLOAT32:
						sp -= 8;
						reg.c = (*(float*)(stack + sp)) <
						(*(float*)(stack + sp+4));
					break;
					case FLOAT64:
						sp -= 16;
						reg.c = (*(double*)(stack + sp)) <
						(*(double*)(stack + sp+8));
					break;
				}
				stack[sp++] = reg.c;
				//SilentPushBack(stackT, dt + UINT8);
				stackT[stackTPtr++] = UINT8;
			break;

			case LargerThan:
				// SilentPopBack(stackT);
				// SilentPopBack(stackT);
				// switch(stackT->data[stackT->ptr])
				stackTPtr--;
				switch(stackT[--stackTPtr])
				{
					case INT8:
						sp-=2;
						reg.c = (*(char*)(stack + sp)) >
						(*(char*)(stack + sp+1));
					break;
					case UINT8:
						sp-=2;
						reg.c = (*(unsigned char*)(stack + sp)) >
						(*(unsigned char*)(stack + sp+1));
					break;
					case INT16:
						sp -= 4;
						reg.c = (*(short*)(stack + sp)) >
						(*(short*)(stack + sp+2));
					break;
					case UINT16:
						sp -= 4;
						reg.c = (*(unsigned short*)(stack + sp)) >
						(*(unsigned short*)(stack + sp+2));
					break;
					case INT32:
						sp -= 8;
						reg.c = (*(int*)(stack + sp)) >
						(*(int*)(stack + sp+4));
					break;
					case UINT32:
						sp -= 8;
						reg.c = (*(unsigned int*)(stack + sp)) >
						(*(unsigned int*)(stack + sp+4));
					break;
					case INT64:
						sp -= 16;
						reg.c = (*(int64*)(stack + sp)) >
						(*(int64*)(stack + sp+8));
					break;
					case UINT64:
					case POINTER:
						sp -= 16;
						reg.c = (*(uint64*)(stack + sp)) >
						(*(uint64*)(stack + sp+8));
					break;
					case FLOAT32:
						sp -= 8;
						reg.c = (*(float*)(stack + sp)) >
						(*(float*)(stack + sp+4));
					break;
					case FLOAT64:
						sp -= 16;
						reg.c = (*(double*)(stack + sp)) >
						(*(double*)(stack + sp+8));
					break;
				}
				stack[sp++] = reg.c;
				//SilentPushBack(stackT, dt + UINT8);
				stackT[stackTPtr++] = UINT8;
			break;

			case Equal:
				// SilentPopBack(stackT);
				// SilentPopBack(stackT);
				// switch(stackT->data[stackT->ptr])
				stackTPtr--;
				switch(stackT[--stackTPtr])
				{
					case INT8:
						sp-=2;
						reg.c = (*(char*)(stack + sp)) ==
						(*(char*)(stack + sp+1));
					break;
					case UINT8:
						sp-=2;
						reg.c = (*(unsigned char*)(stack + sp)) ==
						(*(unsigned char*)(stack + sp+1));
					break;
					case INT16:
						sp -= 4;
						reg.c = (*(short*)(stack + sp)) ==
						(*(short*)(stack + sp+2));
					break;
					case UINT16:
						sp -= 4;
						reg.c = (*(unsigned short*)(stack + sp)) ==
						(*(unsigned short*)(stack + sp+2));
					break;
					case INT32:
						sp -= 8;
						reg.c = (*(int*)(stack + sp)) ==
						(*(int*)(stack + sp+4));
					break;
					case UINT32:
						sp -= 8;
						reg.c = (*(unsigned int*)(stack + sp)) ==
						(*(unsigned int*)(stack + sp+4));
					break;
					case INT64:
						sp -= 16;
						reg.c = (*(int64*)(stack + sp)) ==
						(*(int64*)(stack + sp+8));
					break;
					case UINT64:
					case POINTER:
						sp -= 16;
						reg.c = (*(uint64*)(stack + sp)) ==
						(*(uint64*)(stack + sp+8));
					break;
					case FLOAT32:
						sp -= 8;
						reg.c = (*(float*)(stack + sp)) ==
						(*(float*)(stack + sp+4));
					break;
					case FLOAT64:
						sp -= 16;
						reg.c = (*(double*)(stack + sp)) ==
						(*(double*)(stack + sp+8));
					break;
				}
				//printf("RESULT = %i\n", reg.c);
				stack[sp++] = reg.c;
				//SilentPushBack(stackT, dt + UINT8);
				stackT[stackTPtr++] = UINT8;
			break;

			case If:
				sp--;
				//SilentPopBack(stackT);
				stackTPtr--;
				if(stack[sp])
				{
					pc++;
					pc = *((uint64*)(program + (pc)));
					pc--;
				}
				else pc += 8;
			break;

			case IfNot:
				sp--;
				//SilentPopBack(stackT);
				stackTPtr--;
				if(!stack[sp])
				{
					pc++;
					pc = *((uint64*)(program + (pc)));
					pc--;
				}
				else pc += 8;
			break;

			case And:
				sp--;
				//SilentPopBack(stackT);
				stackTPtr--;
				stack[sp-1] = stack[sp-1] & stack[sp];
			break;

			case Or:
				sp--;
				//SilentPopBack(stackT);
				stackTPtr--;
				stack[sp-1] = stack[sp-1] | stack[sp];
			break;

			case Not:
				stack[sp-1] = !stack[sp-1];
			break;

/*
			case ByteToShort:
				memset(stack + *sp + *fp, 0, 1);
				*sp += 1;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case ByteToInt:
				memset(stack + *sp + *fp, 0, 3);
				*sp += 3;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ByteToLong:
				memset(stack + *sp + *fp, 0, 7);
				*sp += 7;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case ByteToFloat:
				*sp-=1;
				reg.f = *((char*)stack + *sp + *fp);
				memcpy(stack + *sp + *fp, &reg.f, 4);
				*sp += 4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ByteToDouble:
				*sp-=1;
				reg.d = *((char*)stack + *sp + *fp);
				memcpy(stack + *sp + *fp, &reg.d, 8);
				*sp += 8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case ShortToByte:
				*sp-=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case ShortToInt:
				memset(stack + *sp + *fp, 0, 2);
				*sp += 2;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ShortToLong:
				memset(stack + *sp + *fp, 0, 6);
				*sp += 6;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case ShortToFloat:
				*sp-=2;
				reg.f = *((char*)stack + *sp + *fp);
				memcpy(stack + *sp + *fp, &reg.f, 4);
				*sp += 4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ShortToDouble:
				*sp-=2;
				reg.d = *((char*)stack + *sp + *fp);
				memcpy(stack + *sp + *fp, &reg.f, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;



			case IntToByte:
				*sp-=3;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case IntToShort:
				*sp -= 2;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case IntToLong:
				memset(stack + *sp + *fp, 0, 4);
				*sp += 4;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case IntToFloat:
				*sp-=4;
				reg.f = *((char*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.f, 4);
				*sp+=4;
			break;

			case IntToDouble:
				*sp-=4;
				reg.d = *((char*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.f, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;



			case LongToByte:
				*sp -= 7;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case LongToShort:
				*sp -= 6;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case LongToInt:
				*sp -= 4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case LongToFloat:
				*sp-=8;
				reg.f = *((char*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.f, 4);
				*sp+=4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case LongToDouble:
				*sp-=8;
				reg.d = *((char*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.d, 8);
				*sp+=8;
			break;



			case FloatToByte:
				*sp-=4;
				reg.c = *((float*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.c, 1);
				*sp+=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case FloatToShort:
				*sp-=4;
				reg.c = *((float*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.c, 1);
				*sp+=2;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case FloatToInt:
				*sp-=4;
				reg.i = *((float*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.i, 4);
				*sp+=4;
			break;

			case FloatToLong:
				*sp-=4;
				reg.l = *((float*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.i, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case FloatToDouble:
				*sp-=4;
				reg.d = *((float*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.d, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;		
			break;



			case DoubleToByte:
				*sp-=8;
				reg.c = *((double*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.c, 1);
				*sp+=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case DoubleToShort:
				*sp-=8;
				reg.s = *((double*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.s, 2);
				*sp+=2;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case DoubleToInt:
				*sp-=8;
				reg.i = *((double*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.i, 4);
				*sp+=4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case DoubleToLong:
				*sp-=8;
				reg.l = *((double*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.l, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case DoubleToFloat:
				*sp-=8;
				reg.f = *((double*)stack+*sp + *fp);
				memcpy(stack+*sp + *fp, &reg.f, 4);
				*sp+=4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			

			case SmallerThanByte:
				*sp-=1;
				if((*(char*)(stack + *sp-1 + *fp)) < (*(char*)(stack + *sp + *fp)))
				{
					stack[*sp-1 + *fp] = 1;
				}
				else
				{
					stack[*sp-1 + *fp] = 0;
				}
			break;

			case SmallerThanShort:
				*sp-=3;
				if((*(short*)(stack + *sp-1 + *fp)) < (*(short*)(stack + *sp+1 + *fp)))
				{
					stack[*sp-1 + *fp] = 1;
				}
				else
				{
					stack[*sp-1 + *fp] = 0;
				}
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
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
*/
		}

#if STACK_OUTPUT
		uint64 stackPos = 0;
		for(uint i = 0; i < stackTPtr; i++)
		{
			switch(stackT[i])
			{
				case INT8:
				case UINT8:
					printf("%i, ", stack[stackPos]);
					stackPos++;
				break;
				case INT16:
				case UINT16:
					memcpy(&reg.s, stack + stackPos, 2);
					printf("%i, ", reg.s);
					stackPos += 2;
				break;
				case INT32:
				case UINT32:
					memcpy(&reg.i, stack + stackPos, 4);
					printf("%i, ",reg.i);
					stackPos += 4;
				break;
				case INT64:
				case UINT64:
				case POINTER:
					memcpy(&reg.l, stack + stackPos, 8);
					printf("%lli, ",reg.i);
					stackPos += 8;
				break;
				case FLOAT32:
					memcpy(&reg.f, stack + stackPos, 4);
					printf("%f, ",reg.f);
					stackPos += 4;
				break;
				case FLOAT64:
					memcpy(&reg.d, stack + stackPos, 8);
					printf("%f, ",reg.d);
					stackPos += 8;
				break;
			}
		}
		printf("\n");
#endif
		pc++;
		#if DEBUG
			printf("\tpc: %i\n",pc);
			printf("\tstack: %i\n",*(int*)(stack+sp-4));
			//printf("\tstack val: %i\n",stackT->data[stackT->ptr-1]);
			printf("\tstack val: %i\n",stackT[stackTPtr-1]);
			//printf("\tstack count: %i\n",stackT->ptr);
			printf("\tstack count: %i\n",stackTPtr);
			printf("\tstack size: %i\n",sp);
		#endif
	}
}

char markedByte = 1;
void SilentFree(SilentGC* gc, uint64 ptr)
{
	SilentMemory* mem = gc->memory;
	SilentVector* heap = mem->heap;
	((SilentMemoryBlock*)heap->data)[ptr].occupied = 0;
	mem->freeHeapSpace = 1;
}

void SilentSweep(SilentGC* gc)
{
	SilentMemory* mem = gc->memory;
	SilentVector* heap = mem->heap;
	for(uint64 i = 0; i < mem->heapPtr; i++)
	{
		if(((SilentMemoryBlock*)(heap->data))[i].marked != markedByte-2)
		{
			((SilentMemoryBlock*)(heap->data))[i].occupied = 0;
			printf("freeing a thing\n");
			free(((SilentMemoryBlock*)(heap->data))[i].data);
			mem->freeHeapSpace = 1;
		}
	}
}

void SilentMark(SilentGC* gc)
{
	SilentMemory* 	mem = gc->memory;
	char* 			stack = mem->stack;
	SilentVector* 	stackT = mem->stackTypes;
	SilentVector* 	heap = mem->heap;

	uint64 stackPtr = 0;

	for(uint64 i = 0; i < stackT->ptr; i++)
	{
		uint64 temp;
		switch(stackT->data[i])
		{
			case POINTER_LOCATION:
				temp = *((long*)(stack + stackPtr));
				((SilentMemoryBlock*)(heap->data))[temp].marked = markedByte;
				stackPtr += 8;
				printf("Marked a thing\n");
				if(((SilentMemoryBlock*)(heap->data))[temp].occupied == 0)
				{
					free(((SilentMemoryBlock*)(heap->data))[temp].data);
				}
			break;		
			case UNDEFINED:
				temp = *((long*)(stackT->data + i));
				stackPtr += temp;
				i += 8;
			break;
			default:
				stackPtr += SilentGetTypeSize(stackT->data[i]);
			break;
		}
	}
	markedByte += 2;
}

long SilentAlloc(SilentGC* gc, uint64 size)
{
	SilentMemory* mem = gc->memory;
	SilentVector* heap = mem->heap;
	if(mem->freeHeapSpace == 1)
	{
		for(uint64 i = 0; i < mem->heapPtr; i++)
		{
			if(((SilentMemoryBlock*)heap->data)[i].occupied == 0)
			{
				if(((SilentMemoryBlock*)heap->data)[i].data != NULL)
				{	
					free(((SilentMemoryBlock*)heap->data)[i].data);
				}
				((SilentMemoryBlock*)heap->data)[i].occupied = 1;
				((SilentMemoryBlock*)heap->data)[i].data = malloc(size);
				printf("Alloc at %i\n",i);
				return i;
				break;	
			}
		}
		mem->freeHeapSpace = 0;
		return SilentAlloc(gc,size);
	}
	else
	{
		SilentMemoryBlock* memBlock = malloc(sizeof(SilentMemoryBlock));
		memBlock->occupied = 1;
		memBlock->data = malloc(size);
		//SilentPushBack(heap, memBlock);
		mem->heapPtr = heap->ptr/sizeof(SilentMemoryBlock);
		printf("Alloc at %i\n",mem->heapPtr-1);
		printf("alloc ptr: %lu\n",memBlock->data);
		return mem->heapPtr-1;
	}
}