#include "SilentVM2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
	}reg, reg2;

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
	};


	char* 				program = vm->program;
	char* 				stack 	= vm->memory->stack;
	SilentVector*		heap	= vm->memory->heap;
	SilentVector* 		stackT  = vm->memory->stackTypes;
	//SilentVector*		stackF	= vm->memory->stackFrame;

    
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
                running = 0;
				vm->running = 0;
			break;
			
			case Goto:
				pc++;
				pc = *((uint64*)(program + (pc)));
				pc--;
			break;

			case Sweep:
				SilentMark(vm->gc);
				SilentSweep(vm->gc);
			break;


            case Push:
                switch(program[++pc])
                {
                    case INT8:
                        stack[sp++] = program[++pc];
                        SilentPushBack(stackT,dt + INT8);
                    break;
                    case UINT8:
                        stack[sp++] = program[++pc];               
                        SilentPushBack(stackT,dt + UINT8);
                    break;
                    case INT16:
                        memcpy(stack + sp, program + ++pc, 2);
                        sp += 2;
                        pc++;
                        SilentPushBack(stackT,dt + INT16);
                    break;
                    case UINT16:
                        memcpy(stack + sp, program + ++pc, 2);
                        sp += 2;
                        pc++;
                        SilentPushBack(stackT,dt + UINT16);
                    break;
                    case INT32:
                        memcpy(stack + sp, program + ++pc, 4);
                        sp += 4;
                        pc += 3;
                        SilentPushBack(stackT,dt + INT32);
                    break;
                    case UINT32:
                        memcpy(stack + sp, program + ++pc, 4);
                        sp += 4;
                        pc += 3;
                        SilentPushBack(stackT,dt + UINT32);
                    break;
                    case INT64:
                        memcpy(stack + sp, program + ++pc, 8);
                        sp += 8;
                        pc += 7;
                        SilentPushBack(stackT,dt + INT64);
                    break;
                    case UINT64:
                        memcpy(stack + sp, program + ++pc, 8);
                        sp += 8;
                        pc += 7;
                        SilentPushBack(stackT,dt + UINT64);
                    break;
                    case FLOAT32:
                        memcpy(stack + sp, program + ++pc, 4);
                        sp += 4;
                        pc += 3;
                        SilentPushBack(stackT,dt + FLOAT32);
                    break;
                    case FLOAT64:
                        memcpy(stack + sp, program + ++pc, 8);
                        sp += 8;
                        pc += 7;
                        SilentPushBack(stackT,dt + FLOAT64);
                    break;
                    case POINTER:
                        memcpy(stack + sp, program + ++pc, 8);
                        sp += 8;
                        pc += 7;
                        SilentPushBack(stackT,dt + POINTER);
                    break;
                    case UNDEFINED:
                        //Data size
                        memcpy(&reg.l, program + ++pc, 8);
                        pc += 8;
                        //ActualData
                        memcpy(stack + sp, program + pc, reg.l);
                        sp += reg.l;
                        pc += reg.l-1;
                        SilentPushBack(stackT, dt + UNDEFINED);
                        SilentPushMultiple(stackT, 8, &reg.l);
                        SilentPushBack(stackT, dt + UNDEFINED);
                    break;
                }
            break;

            case Pop:
                reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
                if(reg.c != 0)
                {
                    sp -= reg.c;
                    SilentPopBack(stackT);
                }
                else
                {
                    memcpy(&reg.l, stackT->data + (stackT->ptr-9),8);
                    sp -= reg.l;
                    SilentPopMultiple(stackT,10);
                }
            break;

            case Store:
                reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
                memcpy(&reg2.l, program + ++pc, 8);
                pc += 7;
                if(reg.c != 0)
                {
                    memcpy(stack + reg2.l + fp, stack + (sp -= reg.c), reg.c);
                    SilentPopBack(stackT);
                }
                else
                {
                    memcpy(&reg.l, stackT->data + (stackT->ptr-9),8);
                    memcpy(stack + reg2.l + fp, stack + (sp -= reg.l), reg.l);
                    SilentPopMultiple(stackT,10);
                }
            break;

            case Load:
                SilentPushBack(stackT, dt + program[++pc]);
                reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
                if(reg.c != 0)
                {
					//get position
					memcpy(&reg2.l, program + ++pc, 8);
                    pc += 7;
					//load data
                    memcpy(stack + sp, stack + fp + reg2.l, reg.c);
                    sp += reg.c;
                }
                else
                {
					//get type size
                    memcpy(&reg.l, program + ++pc, 8);
                    pc += 8;
                    SilentPushMultiple(stackT,8,&reg.l);
                    SilentPushBack(stackT, dt + UNDEFINED);
					//get position
					memcpy(&reg2.l, program + pc, 8);
					//load data
                    memcpy(stack + sp, stack + fp + reg2.l, reg.l);
                    sp += reg.l;
                    pc += 7;
                }
            break;

            case StoreGlobal:
                reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
                memcpy(&reg2.l, program + ++pc, 8);
                pc += 7;
                if(reg.c != 0)
                {
                    memcpy(program + reg2.l, stack + (sp -= reg.c), reg.c);
                    SilentPopBack(stackT);
                }
                else
                {
                    memcpy(&reg.l, stackT->data + (stackT->ptr-9), 8);
                    memcpy(program + reg2.l, stack + (sp -= reg.l), reg.l);
                    SilentPopMultiple(stackT,10);
                }
            break;

            case LoadGlobal:
                SilentPushBack(stackT, dt + program[++pc]);
                reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
                if(reg.c != 0)
                {
					//get position
                    memcpy(&reg2.l, program + ++pc, 8);
                    pc += 7;
					//load data
                    memcpy(stack + sp, stack + reg2.l, reg.c);
                    sp += reg.c;
                }
                else
                {
					//get type size
                    memcpy(&reg.l, program + ++pc, 8);
                    pc += 8;
                    SilentPushMultiple(stackT,8,&reg.l);
                    SilentPushBack(stackT, dt + UNDEFINED);
					//get position
					memcpy(&reg2.l, program + pc, 8);
					//load data
                    memcpy(stack + sp, stack + reg2.l, reg.l);
                    sp += reg.l;
                    pc += 7;
                }
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
                SilentPushBack(stackT, dt + POINTER_LOCATION);
            break;

            case LoadPtr:
				//get pointer
				sp -= 8;
                memcpy(&tempPtr, stack + sp, 8);
                switch(program[++pc])
                {
                    case INT8:
                        memcpy(stack + sp, tempPtr, 1);
                        sp += 1;
                        SilentPushBack(stackT,dt + INT8);
                    break;
                    case UINT8:
                        memcpy(stack + sp, tempPtr, 1);
                        sp += 1;
                        SilentPushBack(stackT,dt + UINT8);
                    break;
                    case INT16:
                        memcpy(stack + sp, tempPtr, 2);
                        sp += 2;
                        SilentPushBack(stackT,dt + INT16);
                    break;
                    case UINT16:
                        memcpy(stack + sp, tempPtr, 2);
                        sp += 2;
                        SilentPushBack(stackT,dt + UINT16);
                    break;
                    case INT32:
                        memcpy(stack + sp, tempPtr, 4);
                        sp += 4;
                        SilentPushBack(stackT,dt + INT32);
                    break;
                    case UINT32:
                        memcpy(stack + sp, tempPtr, 4);
                        sp += 4;
                        SilentPushBack(stackT,dt + UINT32);
                    break;
                    case INT64:
                        memcpy(stack + sp, tempPtr, 8);
                        sp += 8;
                        SilentPushBack(stackT,dt + INT64);
                    break;
                    case UINT64:
                        memcpy(stack + sp, tempPtr, 8);
                        sp += 8;
                        SilentPushBack(stackT,dt + UINT64);
                    break;
                    case FLOAT32:
                        memcpy(stack + sp, tempPtr, 4);
                        sp += 4;
                        SilentPushBack(stackT,dt + FLOAT32);
                    break;
                    case FLOAT64:
                        memcpy(stack + sp, tempPtr, 8);
                        sp += 8;
                        SilentPushBack(stackT,dt + FLOAT64);
                    break;
                    case POINTER:
                        memcpy(stack + sp, tempPtr, 8);
                        sp += 8;
                        SilentPushBack(stackT,dt + POINTER);
                    break;

                    case UNDEFINED:
                        memcpy(&reg.l, program + ++pc, 8);
						pc += 7;
                        memcpy(stack + sp, tempPtr, reg.l);
                        sp += reg.l;
                        SilentPushBack(stackT, dt + UNDEFINED);
                        SilentPushMultiple(stackT, 8, &reg.l);
                        SilentPushBack(stackT, dt + UNDEFINED);
                    break;
                }
            break;

			case StorePtr:
				//Get pointer
				sp -= 8;
				memcpy(&tempPtr, stack + sp, 8);
				SilentPopBack(stackT);
				reg.c = SilentGetTypeSize(stackT->data[stackT->ptr-1]);
				if(reg.c != 0)
                {
					sp -= reg.c;
					memcpy(&tempPtr, stack + sp, reg.c);
					SilentPopBack(stackT);
                }
                else
                {
					//get data size
					sp -= 8;
					memcpy(&reg.l, stackT->data + (stackT->ptr-9), 8);
					//store data
					sp -= reg.l;
					memcpy(&tempPtr, stack + sp, reg.l);
					SilentPopMultiple(stackT,10);
                }
			break;

			case GetPtr:

				sp -= 8;
				memcpy(reg.l, stack + sp, 8);
				SilentPopBack(stackT);
				tempPtr = ((SilentMemoryBlock*)heap->data)[reg.l].data;
				
			break;
/*
			case StorePtr1:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp + *fp), 8);
				*sp -= 1;
				memcpy(tempPtr, (char*)(stack + *sp + *fp), 1);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtr2:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp + *fp), 8);
				*sp -= 2;
				memcpy(tempPtr, (char*)(stack + *sp + *fp), 2);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtr4:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp + *fp), 8);
				*sp -= 4;
				memcpy(tempPtr, (char*)(stack + *sp + *fp), 4);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtr8:
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp + *fp), 8);
				*sp -= 8;
				memcpy(tempPtr, (char*)(stack + *sp + *fp), 8);
				SilentPopMultiple(stackT, 2);
			break;

			case StorePtrX:
				reg.l = *(uint64*)(vm->program + (++vm->programCounter));
				vm->programCounter += 7;
				*sp -= 8;
				memcpy(&tempPtr, (long*)(stack + *sp + *fp), 8);
				*sp -= reg.l;
				memcpy(tempPtr, (char*)(stack + *sp + *fp), reg.l);
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
				memcpy(stack + *sp + *fp, &reg.l, 8);
				*sp+=8;
				SilentPushBack(stackT,&ds[POINTER]);
			break;

			case AddByte:
				*sp-=1;
				*(char*)(stack + (*sp-1) + *fp) += *(char*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case AddInt:
				*sp-=4;
				*(int*)(stack + (*sp-4) + *fp) += *(int*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;
			
			case AddShort:
				*sp-=2;
				*(short*)(stack+ (*sp-2) + *fp) += *(short*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case AddLong:
				*sp-=8;
				*(int64*)(stack+ (*sp-8) + *fp) += *(int64*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;
			
			case AddFloat:
				*sp-=4;
				*(float*)(stack+ (*sp-4) + *fp) += *(float*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case AddDouble:
				*sp-=8;
				*(double*)(stack+(*sp-8)+ *fp) += *(double*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;



			case SubByte:
				*sp-=1;
				*(char*)(stack + (*sp-1) + *fp) -= *(char*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case SubShort:
				*sp-=2;
				*(short*)(stack + (*sp-2) + *fp) -= *(short*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case SubInt:
				*sp-=4;
				*(int*)(stack + (*sp-4) + *fp) -= *(int*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case SubLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8) + *fp) -= *(int64*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;
			
			case SubFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4) + *fp) -= *(float*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case SubDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8) + *fp) -= *(double*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;
			
			
			
			case MulByte:
				*sp-=1;
				*(char*)(stack + (*sp-1) + *fp) *= *(char*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case MulShort:
				*sp-=2;
				*(short*)(stack + (*sp-1) + *fp) *= *(short*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case MulInt:
				*sp-=4;
				*(int*)(stack + (*sp-4) + *fp) *= *(int*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case MulLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8) + *fp) *= *(int64*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case MulFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4) + *fp) *= *(float*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case MulDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8) + *fp) *= *(double*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;



			case DivByte:
				*sp-=1;
				*(char*)(stack + (*sp-1) + *fp) /= *(char*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case DivShort:
				*sp-=2;
				*(short*)(stack + (*sp-2) + *fp) /= *(short*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case DivInt:
				*sp-=4;
				*(int*)(stack + (*sp-4) + *fp) /= *(int*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case DivLong:
				*sp-=8;
				*(int64*)(stack + (*sp-8) + *fp) /= *(int64*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case DivFloat:
				*sp-=4;
				*(float*)(stack + (*sp-4) + *fp) /= *(float*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;

			case DivDouble:
				*sp-=8;
				*(double*)(stack + (*sp-8) + *fp) /= *(double*)(stack + *sp + *fp);
				SilentPopBack(stackT);
			break;



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



			case If:
				*sp-=1;
				SilentPopBack(stackT);
				if(*(char*)(stack + *sp + *fp))
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
				if(!(*(char*)(stack + *sp + *fp)))
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
            */
		}
		pc++;
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
		SilentPushBack(heap, memBlock);
		mem->heapPtr = heap->ptr/sizeof(SilentMemoryBlock);
		printf("Alloc at %i\n",mem->heapPtr-1);
		return mem->heapPtr-1;
	}
}