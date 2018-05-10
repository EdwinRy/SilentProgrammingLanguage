#include "SilentVM.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

typedef uint uint;
typedef unsigned long long uint64;
typedef long long int64;

SilentMemory* createSilentMemory(
	uint stackBufferSize, uint heapBufferSize, uint stackFrameSize
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
	return memory; 
}

SilentVM* createSilentVM(SilentMemory* memory, char* program, SilentGC gb)
{
	SilentVM* vm 		= malloc(sizeof(SilentVM));
	vm->memory 			= memory;
	vm->program 		= program;
	vm->running 		= 0;
	vm->programCounter 	= 0;
	return vm;
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
	SilentMemoryBlock* 	heap 	= vm->memory->heap;
	SilentMemory*		memory 	= &vm->memory;
	uint64* 			sp 		= &(vm->memory->stackPointer);
	uint64* 			fp 		= &(vm->memory->framePointer);
	uint64 				altSP 	= *sp;
	uint64				altFP	= *fp;

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
			break;

			case Push2:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,2);
				vm->programCounter++;
				*sp += 2;
			break;

			case Push4:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,4);
				vm->programCounter += 3;
				*sp += 4;
			break;
			
			case Push8:
				memcpy(stack + *sp, vm->program + ++vm->programCounter,8);
				vm->programCounter += 7;
				*sp += 8;
			break;

			case PushX:
				reg.l = *((uint64*)(vm->program + ++vm->programCounter));
				memcpy(stack + *sp, vm->program + vm->programCounter + 7, reg.l);
				vm->programCounter += 7+reg.l;
				*sp += reg.l;
			break;
			
			case Pop1:
				*sp -= 1;
			break;

			case Pop2:
				*sp -= 2;
			break;

			case Pop4:
				*sp -= 4;
			break;

			case Pop8:
				*sp -= 8;
			break;

			case PopX:
				*sp -= *((uint64*)(vm->program + (++vm->programCounter)));
				vm->programCounter += 7;
			break;

			case Store1:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 1), 1);
			break;

			case Store2:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 2), 2);
			break;

			case Store4:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg.l, stack + (*sp -= 4), 4);
			break;

			case Store8:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;	
				memcpy(stack + *fp + reg.l, stack + (*sp -= 8), 8);
			break;

			case StoreX:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 8;
				reg2.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *fp + reg2.l, stack + (*sp -= reg.l), reg.l);
			break;
	
			case Load1:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + (*fp + reg.l), stack + *sp, 1);
				*sp += 1;
			break;

			case Load2:
				reg.l = *(uint64*)(vm->program +(++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + (*fp + reg.l), stack + *sp, 2);
				*sp+=2;
			break;

			case Load4:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + (*fp + reg.l), stack + *sp, 4);
				*sp += 4;
			break;

			case Load8:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + (*fp + reg.l), stack + *sp, 8);
				*sp += 8;
			break;

			case LoadX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 8;
				reg2.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + (*fp + reg2.l), stack + *sp, reg.l);
				*sp += reg.l;
			break;

			case Alloc1:
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
				memcpy(stack + *sp, stack + (*sp -= 8), 1);
				*sp += 1;
			break;

			case LoadPtr2:
				memcpy(stack + *sp, stack + (*sp -= 8), 2);
				*sp += 2;
			break;

			case LoadPtr4:
				memcpy(stack + *sp, stack + (*sp -= 8), 4);
				*sp += 4;
			break;

			case LoadPtr8:
				memcpy(stack + *sp, stack + (*sp -= 8), 8);
				*sp += 8;
			break;

			case LoadPtrX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + *sp, stack + (*sp -= reg.l), reg.l);
				*sp += reg.l;
			break;

			case StorePtr1:
				memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtr2:
				memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtr4:
				memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtr8:
				memcpy(stack + (*sp -= 8), stack + (*sp-=1),1);
			break;

			case StorePtrX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				memcpy(stack + (*sp -= 8), stack + (*sp-=1),reg.l);
			break;

			case FREE:
			break;

			case AddByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) += 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			case AddInt:
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) += 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			case AddLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) += 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			case AddFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) += 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			case AddDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) += 
					*(double*)(memory->stack + memory->stackPointer);

			break;

			case SubByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) -= 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			case SubInt:
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) -= 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			case SubLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) -= 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			case SubFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) -= 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			case SubDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) -= 
					*(double*)(memory->stack + memory->stackPointer);

			break;
			
			case MulByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) *= 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			case MulInt:
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) *= 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			case MulLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) *= 
					*(long*)(memory->stack + memory->stackPointer);
			break;

			case MulFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) *= 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			case MulDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) *= 
					*(double*)(memory->stack + memory->stackPointer);
			break;

			case DivByte:
				memory->stackPointer--;
				*(char*)(memory->stack + (memory->stackPointer-1)) /= 
					*(char*)(memory->stack + memory->stackPointer);
			break;

			case DivInt:
				memory->stackPointer-=4;
				*(int*)(memory->stack + (memory->stackPointer-4)) /= 
					*(int*)(memory->stack + memory->stackPointer);
			break;

			case DivLong:
				memory->stackPointer-=sizeof(long);
				*(long*)(memory->stack + (memory->stackPointer-sizeof(long))) /= 
					*(long*)(memory->stack + memory->stackPointer);
			break;


			case DivFloat:
				memory->stackPointer-=4;
				*(float*)(memory->stack + (memory->stackPointer-4)) /= 
					*(float*)(memory->stack + memory->stackPointer);
			break;

			case DivDouble:
				memory->stackPointer-=sizeof(double);
				*(double*)(memory->stack + (memory->stackPointer-sizeof(double))) /= 
					*(double*)(memory->stack + memory->stackPointer);
			break;

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
		}
		vm->programCounter++;
	}
}