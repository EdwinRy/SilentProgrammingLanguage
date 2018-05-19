#include "SilentVM.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

enum dataSize
{
	BYTE_ONE = 0,
	BYTE_TWO,
	BYTE_FOUR,
	BYTE_EIGHT,
	POINTER,
	UNDEFINED
};

SilentMemory* createSilentMemory(
	uint stackBufferSize, uint heapBufferSize
)
{
	SilentMemory* memory 	= malloc(sizeof(SilentMemory));
	memory->stack 			= malloc(stackBufferSize);
	memory->stackSize		= stackBufferSize;
	memory->stackPointer	= 0;
	memory->framePointer	= 0;

	memory->heap			= malloc(heapBufferSize*sizeof(SilentMemoryBlock));
	memory->heapSize		= heapBufferSize;
	memory->heapPointer		= 0;

	memory->stackTypes		= malloc(10000);
	memory->stackTypePtr 	= 0;
	memory->stackTypeSize	= 0;
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
	free(memory->heap);
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


	vm->running = 1;
	char* 				program = vm->program;
	char* 				stack 	= vm->memory->stack;
	uint64* 			sp 		= &(vm->memory->stackPointer);
	uint64* 			fp 		= &(vm->memory->framePointer);
	uint64 				altSP 	= *sp;
	uint64				altFP	= *fp;
	char* 				stackT  = vm->memory->stackTypes;
	uint64*				stp		= &(vm->memory->stackTypePtr);
	//uint64*				sts		= &(vm->memory->stackTypeSize);
	

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
				stackT[*stp] = BYTE_ONE;
				*stp += 1;
			break;

			case Push2:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,2);
				vm->programCounter++;
				*sp += 2;
				stackT[*stp] = BYTE_TWO;
				*stp += 1;
			break;

			case Push4:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,4);
				vm->programCounter += 3;
				*sp += 4;
				stackT[*stp] = BYTE_FOUR;
				*stp += 1;
			break;
			
			case Push8:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,8);
				vm->programCounter += 7;
				*sp += 8;
				stackT[*stp] = BYTE_EIGHT;
				*stp += 1;
			break;

			case PushX:
				reg.l = *((uint64*)(vm->program + ++vm->programCounter));
				memcpy(stack + *sp, vm->program + vm->programCounter + 8,reg.l);
				vm->programCounter += 7+reg.l;
				*sp += reg.l;
				stackT[*stp] = UNDEFINED;
				*stp += 1;
				memcpy(stackT + *stp, &reg.l, 8);
				*stp += 8;
			break;
			
			case Pop1:
				*sp -= 1;
				*stp -= 1;
			break;

			case Pop2:
				*sp -= 2;
				*stp -= 1;
			break;

			case Pop4:
				*sp -= 4;
				*stp -= 1;
			break;

			case Pop8:
				*sp -= 8;
				*stp -= 1;
			break;

			case PopX:
				reg.l = *((uint64*)(vm->program + (++vm->programCounter)));
				*sp -= reg.l;
				*stp -= (reg.l+1);
				vm->programCounter += 7;
			break;

			case Store1:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 1), 1);
				*stp -= 1;
			break;

			case Store2:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 2), 2);
				*stp -= 1;
			break;

			case Store4:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 4), 4);
				*stp -= 1;
			break;

			case Store8:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;	
				memcpy(stack + *fp + reg.l, stack + (*sp -= 8), 8);
				*stp -= 1;
			break;

			case StoreX:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				reg2.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg2.l, stack + (*sp -= reg.l), reg.l);
				*stp -= 1;
			break;
	
			case Load1:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 1);
				*sp += 1;
				stackT[*stp] = BYTE_ONE;
				*stp += 1;
			break;

			case Load2:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 2);
				*sp+=2;
				stackT[*stp] = BYTE_ONE;
				*stp += 1;
			break;

			case Load4:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 4);
				*sp += 4;
				stackT[*stp] = BYTE_FOUR;
				*stp += 1;
			break;

			case Load8:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + *fp + reg.l, 8);
				*sp += 8;
				stackT[*stp] = BYTE_EIGHT;
				*stp += 1;
			break;

			case LoadX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				reg2.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + (*fp + reg2.l), reg.l);
				*sp += reg.l;
				stackT[*stp] = UNDEFINED;
				*stp += 1;
				memcpy(stackT + *stp, &reg.l, 8);
				*stp += 8;
			break;

			case Alloc1:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				uint64* ptr = SilentAlloc(vm->gc,reg.l);
				memcpy(stack + *sp, &ptr, 8);
			break;

			case Alloc2:
			break;

			case Alloc4:				
			break;

			case Alloc8:
			break;

			case AllocX:	
			break;
			
			case LoadPtr1:
				*sp -= 8;
				memcpy(
					stack + *sp,
					((SilentMemoryBlock*)(stack + *sp))->data,
					1
				);
				*sp += 1;
			break;

			case LoadPtr2:
				*sp -= 8;
				memcpy(
					stack + *sp,
					((SilentMemoryBlock*)(stack + *sp))->data,
					2
				);
				*sp += 2;
			break;

			case LoadPtr4:
				*sp -= 8;
				memcpy(
					stack + *sp,
					((SilentMemoryBlock*)(stack + *sp))->data,
					4
				);
				*sp += 4;
			break;

			case LoadPtr8:
				*sp -= 8;
				memcpy(
					stack + *sp,
					((SilentMemoryBlock*)(stack + *sp))->data,
					8
				);
				*sp += 8;
			break;

			case LoadPtrX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				*sp -= reg.l;
				memcpy(
					stack + *sp,
					((SilentMemoryBlock*)(stack + *sp))->data,
					reg.l
				);
				*sp += reg.l;
			break;

			case StorePtr1:
				//memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtr2:
				//memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtr4:
				//memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtr8:
				//memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtrX:
				//reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				//vm->programCounter += 7;
				//memcpy(stack + (*sp -= 8), stack + (*sp-=1),reg.l);
			break;

			case FREE:
			break;

			

			case AddByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) += *(char*)(stack + *sp);
				*stp -= 1;
			break;

			case AddInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) += *(int*)(stack + *sp);
				*stp -= 1;
			break;
			
			case AddShort:
				*sp-=2;
				*(short*)(stack + (*sp-2)) += *(short*)(stack + *sp);
				*stp -= 1;
			break;

			case AddLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) += *(int64*)(stack + *sp);
				*stp -= 1;
			break;
			
			case AddFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) += *(float*)(stack + *sp);
				*stp -= 1;
			break;

			case AddDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) += *(double*)(stack + *sp);
				*stp -= 1;
			break;



			case SubByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) -= *(char*)(stack + *sp);
				*stp -= 1;
			break;

			case SubShort:
				*sp-=2;
				*(short*)(stack + (*sp-2)) -= *(short*)(stack + *sp);
				*stp -= 1;
			break;

			case SubInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) -= *(int*)(stack + *sp);
				*stp -= 1;
			break;

			case SubLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) -= *(int64*)(stack + *sp);
				*stp -= 1;
			break;
			
			case SubFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) -= *(float*)(stack + *sp);
				*stp -= 1;
			break;

			case SubDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) -= *(double*)(stack + *sp);
				*stp -= 1;
			break;
			
			
			
			case MulByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) *= *(char*)(stack + *sp);
				*stp -= 1;
			break;

			case MulShort:
				*sp-=2;
				*(short*)(stack + (*sp-1)) *= *(short*)(stack + *sp);
				*stp -= 1;
			break;

			case MulInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) *= *(int*)(stack + *sp);
				*stp -= 1;
			break;

			case MulLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) *= *(int64*)(stack + *sp);
				*stp -= 1;
			break;

			case MulFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) *= *(float*)(stack + *sp);
				*stp -= 1;
			break;

			case MulDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) *= *(double*)(stack + *sp);
				*stp -= 1;
			break;



			case DivByte:
				*sp-=1;
				*(char*)(stack + (*sp-1)) /= *(char*)(stack + *sp);
				*stp -= 1;
			break;

			case DivShort:
				*sp-=2;
				*(short*)(stack + (*sp-2)) /= *(short*)(stack + *sp);
				*stp -= 1;
			break;

			case DivInt:
				*sp-=4;
				*(int*)(stack + (*sp-4)) /= *(int*)(stack + *sp);
				*stp -= 1;
			break;

			case DivLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8)) /= *(int64*)(stack + *sp);
				*stp -= 1;
			break;

			case DivFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4)) /= *(float*)(stack + *sp);
				*stp -= 1;
			break;

			case DivDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8)) /= *(double*)(stack + *sp);
				*stp -= 1;
			break;


/*
			case ByteToInt:
				memset(memory->stack + memory->stackPointer, 0, 3);
				memory->stackPointer += 3;
			break;

			case ByteToLong:
				memset(memory->stack + memory->stackPointer, 0, sizeof(long)-1);
				memory->stackPointer += sizeof(long)-1;
			break;

			case ByteToFloat:
				memory->stackPointer -= 1;
				breg =  *(char*)(memory->stack + (memory->stackPointer));
				freg = breg;
				memcpy(memory->stack + memory->stackPointer, &freg, 4);
				memory->stackPointer += 4;
			break;

			case ByteToDouble:
				memory->stackPointer -= 1;
				breg =  *(char*)(memory->stack + (memory->stackPointer));
				dreg = breg;
				memcpy(memory->stack + memory->stackPointer, &dreg, sizeof(double));
				memory->stackPointer += sizeof(double);
			break;

			case IntToByte:
				memory->stackPointer-=3;
			break;

			case IntToLong:
				memset(memory->stack + memory->stackPointer,0,sizeof(long)-sizeof(int));
				memory->stackPointer+=sizeof(long)-sizeof(int);
			break;
			
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
				reg.l = (long)freg;
				memcpy(memory->stack + (memory->stackPointer), &reg.l, sizeof(long));
				memory->stackPointer+=sizeof(long);
			break;

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
				reg.l = (long)dreg;
				memcpy(memory->stack + (memory->stackPointer), &reg.l, sizeof(long));
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
				if(*(char*)(memory->stack + --memory->stackPointer))
				{
					vm->programCounter++;
					vm->programCounter = 
					*((uint*)(vm->program + (vm->programCounter)));
					vm->programCounter--;
				}
				else
				{
					vm->programCounter += 4;
				}
			break;


			case IfNot:
				if(!(*(char*)(memory->stack + (--memory->stackPointer))))
				{
					vm->programCounter++;
					vm->programCounter = 
					*((uint*)(vm->program + (vm->programCounter)));
					vm->programCounter--;
				}
				else
				{
					vm->programCounter += 4;
				}
			break;
			*/	
		}
		vm->programCounter++;
	}
}

void SilentSweep(SilentGC* gc)
{

}

void SilentFree(SilentGC* gc, uint64* ptr)
{

}

void* SilentAlloc(SilentGC* gc, uint64 size)
{
    SilentMemory* mem = gc->memory;
    if(mem->heapSize - mem->heapPointer < 2)
    {
        SilentSweep(gc);
        if(mem->heapSize - mem->heapPointer < 2)
        {
            mem->heapSize = ceil(mem->heapSize/10)+mem->heapSize;
            void* temp = realloc(mem->heap,mem->heapSize);
            if(temp != NULL)
            {
                mem->heap = temp;
            }
            else
            {
                printf("Couldn't allocate more heap memory\n");
                printf("Press any key to retry...\n");
                getchar();
                return SilentAlloc(gc,size);
            } 
        }
    }
    for(uint64 i = 0; i < mem->heapSize; i++)
    {
        if(mem->heap[i].occupied == 0)
        {
            mem->heap[i].data = malloc(size);
            return mem->heap + i;
            break;
        }
    }
    return NULL;
}