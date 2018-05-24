#include "SilentVM.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

typedef enum dataSize
{
	BYTE_ONE = 0,
	BYTE_TWO,
	BYTE_FOUR,
	BYTE_EIGHT,
	POINTER,
	POINTER_LOCATION,
	UNDEFINED,
	UNDEFINED_END
}dataSize;

SilentMemory* createSilentMemory(
	uint stackBufferSize, uint heapBufferSize
)
{
	SilentMemory* memory 	= malloc(sizeof(SilentMemory));
	memory->stack 			= malloc(stackBufferSize);
	memory->stackSize		= stackBufferSize;
	memory->stackPointer	= 0;
	memory->framePointer	= 0;

	memory->heap			= SilentCreateVector(heapBufferSize, 8);
	memory->stackTypes		= SilentCreateVector(stackBufferSize/4, 1);
	memory->stackFrame		= SilentCreateVector(stackBufferSize/4, 8);
	return memory; 
}

SilentVM* createSilentVM(SilentMemory* memory, char* program, SilentGC* gc)
{
	SilentVM* vm 		= malloc(sizeof(SilentVM));
	vm->memory 			= memory;
	vm->program 		= program;
	vm->running 		= 0;
	vm->programCounter 	= 0;
	vm->gc = gc;
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
	}reg, reg2;

	dataSize ds[] = {
		BYTE_ONE,
		BYTE_TWO,
		BYTE_FOUR,
		BYTE_EIGHT,
		POINTER,
		POINTER_LOCATION,
		UNDEFINED,UNDEFINED_END
	};

	vm->running = 1;
	char* 				program = vm->program;
	char* 				stack 	= vm->memory->stack;
	uint64* 			sp 		= &(vm->memory->stackPointer);
	uint64* 			fp 		= &(vm->memory->framePointer);
	uint64 				altSP 	= *sp;
	uint64				altFP	= *fp;
	SilentVector*		heap	= vm->memory->heap;
	SilentVector* 		stackT  = vm->memory->stackTypes;

	char* tempPtr;

	while(vm->running)
	{
		switch(program[vm->programCounter])
		{
			case Halt:
				vm->running = 0;
			break;
			
			case Goto:
				vm->programCounter++;
				vm->programCounter = 
					*((uint64*)(vm->program + (vm->programCounter)));
				vm->programCounter--;
			break;

			case Sweep:
				SilentMark(vm->gc);
				SilentSweep(vm->gc);
			break;

			case UseGlobal:
				altSP = *sp;
				altFP = *fp;
				*sp = 0;
				*fp = 0;
			break;

			//Call silent subrouting
			case EndGlobal:
				*sp = altSP;
				*fp = altFP;
			break;

			case Call:

			break;

			case Return:

			break;

			case Push1:
				stack[*sp] = vm->program[++(vm->programCounter)];
				*sp += 1;
				SilentPushBack(stackT, &ds[BYTE_ONE]);
			break;

			case Push2:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,2);
				vm->programCounter++;
				*sp += 2;
				SilentPushBack(stackT, &ds[BYTE_TWO]);
			break;

			case Push4:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,4);
				vm->programCounter += 3;
				*sp += 4;
				SilentPushBack(stackT, &ds[BYTE_FOUR]);
			break;
			
			case Push8:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,8);
				vm->programCounter += 7;
				*sp += 8;
				SilentPushBack(stackT, &ds[BYTE_EIGHT]);
			break;

			case PushX:
				reg.l = *((uint64*)(vm->program + ++vm->programCounter));
				memcpy(stack + *sp, vm->program + vm->programCounter + 8,reg.l);
				vm->programCounter += 7+reg.l;
				*sp += reg.l;
				SilentPushBack(stackT, &ds[UNDEFINED]);
				SilentPushMultiple(stackT,8,&reg.l);
				SilentPushBack(stackT, &ds[UNDEFINED_END]);
			break;
			
			case Pop1:
				*sp -= 1;
				SilentPopBack(stackT);
			break;

			case Pop2:
				*sp -= 2;
				SilentPopBack(stackT);
			break;

			case Pop4:
				*sp -= 4;
				SilentPopBack(stackT);
			break;

			case Pop8:
				*sp -= 8;
				SilentPopBack(stackT);
			break;

			case PopX://Fix for multiple values
				reg.l = *((uint64*)(vm->program + (++vm->programCounter)));
				*sp -= reg.l;
				SilentPopMultiple(stackT,reg.l+2);
				vm->programCounter += 7;
			break;

			case Store1:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 1), 1);
				SilentPopBack(stackT);
			break;

			case Store2:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 2), 2);
				SilentPopBack(stackT);
			break;

			case Store4:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 4), 4);
				SilentPopBack(stackT);
			break;

			case Store8:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;	
				memcpy(stack + *fp + reg.l, stack + (*sp -= 8), 8);
				SilentPopBack(stackT);
			break;

			case StoreX:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				reg2.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg2.l, stack + (*sp -= reg.l), reg.l);
				SilentPopBack(stackT);
			break;
	
			case Load1:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 1);
				*sp += 1;
				SilentPushBack(stackT, &ds[BYTE_ONE]);
			break;

			case Load2:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 2);
				*sp+=2;
				SilentPushBack(stackT, &ds[BYTE_TWO]);
			break;

			case Load4:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 4);
				*sp += 4;
				SilentPushBack(stackT, &ds[BYTE_FOUR]);
			break;

			case Load8:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 8);
				*sp += 8;
				SilentPushBack(stackT, &ds[BYTE_EIGHT]);
			break;

			case LoadX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				reg2.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + (*fp + reg2.l), reg.l);
				*sp += reg.l;
				SilentPushBack(stackT, &ds[UNDEFINED]);
				SilentPushMultiple(stackT, 8, &reg.l);
				SilentPushBack(stackT, &ds[UNDEFINED_END]);
			break;

			case Alloc1:
				reg.l = SilentAlloc(vm->gc, 1);
				memcpy(stack + *sp, &reg.l, 8);
				*sp += 8;
				SilentPushBack(stackT,&ds[POINTER_LOCATION]);
			break;

			case Alloc2:
				reg.l = SilentAlloc(vm->gc, 2);
				memcpy(stack + *sp, &reg.l, 8);
				*sp += 8;
				SilentPushBack(stackT,&ds[POINTER_LOCATION]);
			break;

			case Alloc4:
				reg.l = SilentAlloc(vm->gc, 4);
				memcpy(stack + *sp, &reg.l, 8);
				*sp += 8;
				SilentPushBack(stackT,&ds[POINTER_LOCATION]);		
			break;

			case Alloc8:
				reg.l = SilentAlloc(vm->gc, 8);
				memcpy(stack + *sp, &reg.l, 8);
				*sp += 8;
				SilentPushBack(stackT,&ds[POINTER_LOCATION]);
			break;

			case AllocX:
				reg2.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				reg.l = SilentAlloc(vm->gc, 1);
				memcpy(stack + *sp, &reg.l, reg2.l);
				*sp += 8;
				SilentPushBack(stackT,&ds[POINTER_LOCATION]);
			break;

			case LoadPtr1:
				*sp -= 8;
				SilentPopBack(stackT);
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				memcpy(stack + *sp, tempPtr, 1);
				*sp += 1;
				SilentPushBack(stackT,&ds[BYTE_ONE]);
			break;

			case LoadPtr2:
				*sp -= 8;
				SilentPopBack(stackT);
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				memcpy(stack + *sp, tempPtr, 2);
				*sp += 2;
				SilentPushBack(stackT,&ds[BYTE_TWO]);
			break;

			case LoadPtr4:
				*sp -= 8;
				SilentPopBack(stackT);
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				memcpy(stack + *sp, tempPtr, 4);
				*sp += 4;
				SilentPushBack(stackT,&ds[BYTE_FOUR]);
			break;

			case LoadPtr8:
				*sp -= 8;
				SilentPopBack(stackT);
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				memcpy(stack + *sp, tempPtr, 8);
				*sp += 8;
				SilentPushBack(stackT,&ds[BYTE_EIGHT]);
			break;

			case LoadPtrX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				*sp -= 8;
				SilentPopBack(stackT);
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				memcpy(stack + *sp, tempPtr, reg.l);//
				*sp += reg.l;
				SilentPushBack(stackT,&ds[UNDEFINED]);
				SilentPushMultiple(stackT,8,&reg.l);
				SilentPushBack(stackT,&ds[UNDEFINED_END]);
			break;

			case StorePtr1:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				*sp -= 1;
				memcpy(tempPtr, (char*)(stack + *sp), 1);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtr2:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				*sp -= 2;
				memcpy(tempPtr, (char*)(stack + *sp), 2);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtr4:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				*sp -= 4;
				memcpy(tempPtr, (char*)(stack + *sp), 4);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtr8:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				*sp -= 8;
				memcpy(tempPtr, (char*)(stack + *sp), 8);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtrX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp), 8);
				*sp -= reg.l;
				memcpy(tempPtr, (char*)(stack + *sp), reg.l);
				SilentPopMultiple(stackT, 10);
			break;

			case FREE:
				reg2.l = *((uint64*)(vm->program + ++vm->programCounter));
				reg.l = *((long*)(stack + *fp + reg2.l));
				vm->programCounter += 7;
				SilentFree(vm->gc, reg.l);
			break;

			case GetPtr:
				reg2.l = *((uint64*)(vm->program + ++vm->programCounter));
				reg.l = *((long*)(stack + *fp + reg2.l));
				vm->programCounter += 7;
				reg.l = (long)((SilentMemoryBlock*)heap->data)[reg.l].data;
				memcpy(stack+*sp, &reg.l, 8);
				*sp+=8;
				SilentPushBack(stackT,&ds[POINTER]);
			break;

			case AddByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) += *(char*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case AddInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) += *(int*)(stack + *sp);
				SilentPopBack(stackT);
			break;
			
			case AddShort:
				*sp-=2;
				*(short*)(stack + (*sp-2)) += *(short*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case AddLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) += *(int64*)(stack + *sp);
				SilentPopBack(stackT);
			break;
			
			case AddFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) += *(float*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case AddDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) += *(double*)(stack + *sp);
				SilentPopBack(stackT);
			break;



			case SubByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) -= *(char*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case SubShort:
				*sp-=2;
				*(short*)(stack + (*sp-2)) -= *(short*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case SubInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) -= *(int*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case SubLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) -= *(int64*)(stack + *sp);
				SilentPopBack(stackT);
			break;
			
			case SubFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) -= *(float*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case SubDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) -= *(double*)(stack + *sp);
				SilentPopBack(stackT);
			break;
			
			
			
			case MulByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) *= *(char*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case MulShort:
				*sp-=2;
				*(short*)(stack + (*sp-1)) *= *(short*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case MulInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) *= *(int*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case MulLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) *= *(int64*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case MulFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) *= *(float*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case MulDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) *= *(double*)(stack + *sp);
				SilentPopBack(stackT);
			break;



			case DivByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) /= *(char*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case DivShort:
				*sp-=2;
				*(short*)(stack + (*sp-2)) /= *(short*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case DivInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) /= *(int*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case DivLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) /= *(int64*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case DivFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) /= *(float*)(stack + *sp);
				SilentPopBack(stackT);
			break;

			case DivDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) /= *(double*)(stack + *sp);
				SilentPopBack(stackT);
			break;



			case ByteToShort:
				memset(stack + *sp, 0, 1);
				*sp += 1;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case ByteToInt:
				memset(stack + *sp, 0, 3);
				*sp += 3;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ByteToLong:
				memset(stack + *sp, 0, 7);
				*sp += 7;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case ByteToFloat:
				*sp-=1;
				reg.f = *((char*)stack + *sp);
				memcpy(stack + *sp, &reg.f, 4);
				*sp += 4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ByteToDouble:
				*sp-=1;
				reg.d = *((char*)stack + *sp);
				memcpy(stack + *sp, &reg.d, 8);
				*sp += 8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;



			case ShortToByte:
				*sp-=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case ShortToInt:
				memset(stack + *sp, 0, 2);
				*sp += 2;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ShortToLong:
				memset(stack + *sp, 0, 6);
				*sp += 6;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case ShortToFloat:
				*sp-=2;
				reg.f = *((char*)stack + *sp);
				memcpy(stack + *sp, &reg.f, 4);
				*sp += 4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case ShortToDouble:
				*sp-=2;
				reg.d = *((char*)stack + *sp);
				memcpy(stack + *sp, &reg.f, 8);
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
				memset(stack + *sp, 0, 4);
				*sp += 4;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case IntToFloat:
				*sp-=4;
				reg.f = *((char*)stack+*sp);
				memcpy(stack+*sp, &reg.f, 4);
				*sp+=4;
			break;

			case IntToDouble:
				*sp-=4;
				reg.d = *((char*)stack+*sp);
				memcpy(stack+*sp, &reg.f, 8);
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
				reg.f = *((char*)stack+*sp);
				memcpy(stack+*sp, &reg.f, 4);
				*sp+=4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case LongToDouble:
				*sp-=8;
				reg.d = *((char*)stack+*sp);
				memcpy(stack+*sp, &reg.d, 8);
				*sp+=8;
			break;



			case FloatToByte:
				*sp-=4;
				reg.c = *((float*)stack+*sp);
				memcpy(stack+*sp, &reg.c, 1);
				*sp+=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case FloatToShort:
				*sp-=4;
				reg.c = *((float*)stack+*sp);
				memcpy(stack+*sp, &reg.c, 1);
				*sp+=2;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case FloatToInt:
				*sp-=4;
				reg.i = *((float*)stack+*sp);
				memcpy(stack+*sp, &reg.i, 4);
				*sp+=4;
			break;

			case FloatToLong:
				*sp-=4;
				reg.l = *((float*)stack+*sp);
				memcpy(stack+*sp, &reg.i, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case FloatToDouble:
				*sp-=4;
				reg.d = *((float*)stack+*sp);
				memcpy(stack+*sp, &reg.d, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;		
			break;



			case DoubleToByte:
				*sp-=8;
				reg.c = *((double*)stack+*sp);
				memcpy(stack+*sp, &reg.c, 1);
				*sp+=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;

			case DoubleToShort:
				*sp-=8;
				reg.s = *((double*)stack+*sp);
				memcpy(stack+*sp, &reg.s, 2);
				*sp+=2;
				stackT->data[stackT->ptr - 1] = BYTE_TWO;
			break;

			case DoubleToInt:
				*sp-=8;
				reg.i = *((double*)stack+*sp);
				memcpy(stack+*sp, &reg.i, 4);
				*sp+=4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			case DoubleToLong:
				*sp-=8;
				reg.l = *((double*)stack+*sp);
				memcpy(stack+*sp, &reg.l, 8);
				*sp+=8;
				stackT->data[stackT->ptr - 1] = BYTE_EIGHT;
			break;

			case DoubleToFloat:
				*sp-=8;
				reg.f = *((double*)stack+*sp);
				memcpy(stack+*sp, &reg.f, 4);
				*sp+=4;
				stackT->data[stackT->ptr - 1] = BYTE_FOUR;
			break;

			

			case SmallerThanByte:
				*sp-=1;
				if((*(char*)(stack + *sp-1)) < (*(char*)(stack + *sp)))
				{
					stack[*sp-1] = 1;
				}
				else
				{
					stack[*sp-1] = 0;
				}
				//*stp-=1;
			break;

			case SmallerThanShort:
				*sp-=3;
				if((*(short*)(stack + *sp-1)) < (*(short*)(stack + *sp+1)))
				{
					stack[*sp-1] = 1;
				}
				else
				{
					stack[*sp-1] = 0;
				}
				//*stp-=1;
				stackT->data[stackT->ptr - 1] = BYTE_ONE;
			break;
/*
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


			case If:
				*sp-=1;
				SilentPopBack(stackT);
				if(*(char*)(stack + *sp))
				{
					vm->programCounter++;
					vm->programCounter = 
					*((uint*)(vm->program + (vm->programCounter)));
					vm->programCounter--;
				}
				else
				{
					vm->programCounter += 8;
				}
			break;


			case IfNot:
				*sp-=1;
				SilentPopBack(stackT);
				if(!(*(char*)(stack + *sp)))
				{
					vm->programCounter++;
					vm->programCounter = 
					*((uint*)(vm->program + (vm->programCounter)));
					vm->programCounter--;
				}
				else
				{
					vm->programCounter += 8;
				}
			break;
		}
		vm->programCounter++;
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
			break;		
			case UNDEFINED:
				temp = *((long*)(stackT->data + i));
				stackPtr += temp;
				i += 8;
			break;
			case BYTE_ONE:stackPtr += 1;break;
			case BYTE_TWO:stackPtr += 2;break;
			case BYTE_FOUR:stackPtr += 4;break;
			case BYTE_EIGHT:stackPtr += 8;break;
			case POINTER:stackPtr += 8;break;
			default:break;
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
		SilentPushBack(heap, memBlock);
		mem->heapPtr = heap->ptr/sizeof(SilentMemoryBlock);
		printf("Alloc at %i\n",mem->heapPtr-1);
		return mem->heapPtr-1;
	}
}