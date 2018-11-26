#include "SilentVM.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned long long uint64;
typedef long long int64;
void SilentStartVM(char* prog)
{
    volatile char* stack = malloc(10000);
	register char* program = prog;
	//char* program = prog;
    uint64 sp = 0;	//stack pointer
    uint64 fp = 0;	//frame pointer
    register uint64 pc = 0;	//program counter

    uint64 *saveSfData = malloc(90000); //old stack frame data
    register uint64 saveSfDataPtr = 0;

	SilentGC gc;
	gc.heap = malloc(1000 * sizeof(SilentMemoryBlock));
	gc.heapPtr = 0;
	gc.lastFree = 0;

    typedef union Registers
	{
		char c;
		short s;
		int i;
		long long l;
		float f;
		double d;
	}Registers;//, reg3, reg4;

	volatile Registers reg;
	volatile Registers reg2;

    //char running = 1;

    //while(running)
	//Turns out for(;;) is somehow faster (I assume it's because you dont have)
	//To check for the variable running each time
	for(;;)
    {
		//Check for the current instruction being executed
        switch (*(program + pc))
        {
			//Exit out of the execution loop
            case Halt:
                //running = 0;
				goto endLoop;
            break;
        
			//Update program counter with the provided argument
            case Goto:
                pc = *((uint64*)(program + (pc+1)));
                pc--;//Decrement pc as it's increased before next iteration
            break;

			//Call a subroutine in the program (saves stack frame data)
			//And moves to a new position in the program
            case Call:
                //Get goto location
                reg.l = *((uint64*)(program + (++pc)));
                pc += 8;
                //Get arg size
                reg2.l = *((uint64*)(program + (pc)));
                pc += 7;
                //save frame pointer
                saveSfData[saveSfDataPtr++] = fp;
                fp = sp;
                fp -= reg2.l;
                //save stack pointer
                saveSfData[saveSfDataPtr++] = sp - reg2.l;
                //save return position
                saveSfData[saveSfDataPtr++] = pc;
                pc = reg.l - 1;
            break;

			//Copy return values to the old stack pointer and return to 
			//old subroutine
            case Return:
                //Get return size
                reg.l = *((uint64*)(program + (++pc)));
                pc += 7;
                //Get old program counter
                pc = saveSfData[--saveSfDataPtr];
                //Get old stack pointer
                reg2.l = sp;
                sp = saveSfData[--saveSfDataPtr];
                memcpy(stack + sp, stack + reg2.l - reg.l, reg.l);
                sp += reg.l;
                //Get old frame pointer
                fp = saveSfData[--saveSfDataPtr];
            break;

			//Push 1 byte from the program onto the stack
            case Push1:
                stack[sp++] = program[++pc];
            break;

			//Push 2 bytes from the program onto the stack
            case Push2:
                memcpy(stack + sp, program + (++pc), 2);
                sp += 2;
                pc++;
            break;

            case Push4:
				memcpy(stack + sp, program + (++pc), 4);
				sp += 4;
				pc += 3;
			break;

			case Push8:
				//memcpy(stack + sp, program + (++pc), 8);
				(*(uint64*)(stack + sp)) = (*(uint64*)(program + (++pc)));
				sp += 8;
				pc += 7;
			break;

			case Pop1:
				sp--;
			break;

			case Pop2:
				sp -= 2;
			break;

			case Pop4:
				sp -= 4;
			break;

			case Pop8:
				sp -= 8;
			break;

			case Store1:
				//memcpy(&reg2.l, program + (++pc), 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//memcpy(stack + fp + reg2.l, stack + (--sp), 1);
                stack[fp + reg.l] = stack[--sp];
			break;

			case Store2:
				//memcpy(&reg2.l, program + (++pc), 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= 2;
				memcpy(stack + fp + reg.l, stack + sp, 2);
			break;

			case Store4:
				//memcpy(&reg2.l, program + (++pc), 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= 4;
				memcpy(stack + fp + reg.l, stack + sp, 4);
			break;

			case Store8:
				//memcpy(&reg2.l, program + (++pc), 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= 8;
				memcpy(stack + fp + reg.l, stack + sp, 8);
			break;

			case Load1:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//memcpy(stack + sp, stack + fp + reg.l, 1);
				(*(char*)(stack + sp)) = (*(char*)(stack + fp + reg.l));
				sp++;
			break;

			case Load2:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//memcpy(stack + sp, stack + fp + reg.l, 2);
				(*(short*)(stack + sp)) = (*(short*)(stack + fp + reg.l));
				sp += 2;
			break;

			case Load4:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//memcpy(stack + sp, stack + fp + reg.l, 4);
				(*(int*)(stack + sp)) = (*(int*)(stack + fp + reg.l));
				sp += 4;
			break;

			case Load8:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//memcpy(stack + sp, stack + fp + reg.l, 8);
				(*(uint64*)(stack + sp)) = (*(uint64*)(stack + fp + reg.l));
				sp += 8;
			break;

			case StoreGlobal1:
				//Get global offset
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 1;
				memcpy(stack + reg.l, stack + sp, 1);
			break;
			
			case StoreGlobal2:
				//Get global offset
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 2;
				memcpy(stack + reg.l, stack + sp, 2);
			break;

			case StoreGlobal4:
				//Get global offset
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 4;
				memcpy(stack + reg.l, stack + sp, 4);
			break;

			case StoreGlobal8:
				//Get global offset
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 8;
				memcpy(stack + reg.l, stack + sp, 8);
			break;

			case LoadGlobal1:
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 1);
				sp++;
			break;

			case LoadGlobal2:
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 2);
				sp += 2;
			break;

			case LoadGlobal4:
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 4);
				sp += 4;
			break;

			case LoadGlobal8:
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 8);
				sp += 8;
			break;

			case Alloc1:
			{
				uint64 temp = SilentAlloc(&gc, 1);
				pc += 7;
				memcpy(stack + sp, &temp, 8);
				sp += 8;
			}
			break;

			case Alloc2:
			{
				uint64 temp = SilentAlloc(&gc, 2);
				pc += 7;
				memcpy(stack + sp, &temp, 8);
				sp += 8;
			}
			break;

			case Alloc4:
			{
				uint64 temp = SilentAlloc(&gc, 4);
				pc += 7;
				memcpy(stack + sp, &temp, 8);
				sp += 8;
			}
			break;

			case Alloc8:
			{
				uint64 temp = SilentAlloc(&gc, 8);
				pc += 7;
				memcpy(stack + sp, &temp, 8);
				sp += 8;
			}
			break;

			case AllocX:
			{
				//Get alloc size
				uint64 temp = 
					SilentAlloc(&gc, (uint64)*((uint64*)(program+(++pc))));
				pc += 7;
				memcpy(stack + sp, &temp, 8);
				sp += 8;
			}
			break;

			case LoadPtr1:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack+sp, gc.heap[reg.l].data + reg2.l, 1);
				sp++;
			break;

			case LoadPtr2:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack+sp, gc.heap[reg.l].data + reg2.l, 2);
				sp += 2;
			break;

			case LoadPtr4:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack+sp, gc.heap[reg.l].data + reg2.l, 4);
				sp += 4;
			break;

			case LoadPtr8:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack+sp, gc.heap[reg.l].data + reg2.l, 8);
				sp += 8;
			break;

			case LoadPtrX:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 8;
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;

				memcpy(stack+sp, 
					gc.heap[(uint64)*((uint64*)(stack + (sp)))].data + reg2.l, 
					reg.l);
				sp += reg.l;
			break;

			case StorePtr1:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp--;
				memcpy(gc.heap[reg.l].data + reg2.l, stack+sp, 1);
			break;

			case StorePtr2:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= 2;
				memcpy(gc.heap[reg.l].data + reg2.l, stack+sp, 2);
			break;

			case StorePtr4:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= 4;
				memcpy(gc.heap[reg.l].data + reg2.l, stack+sp, 7);
			break;

			case StorePtr8:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= 8;
				memcpy(gc.heap[reg.l].data + reg2.l, stack+sp, 8);
			break;

			case StorePtrX:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 8;
				reg2.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				sp -= reg.l;
				memcpy(gc.heap[(uint64)*((uint64*)(stack+(sp)))].data + reg2.l, 
					stack+sp, reg.l);
			break;

			case Free:
				SilentFree(&gc, (uint64)*((uint64*)(program + (++pc))));
				pc += 7;
			break;

			case GetPtr:
				sp -= 8;
				reg.l = (uint64)*((uint64*)(stack + (sp)));
				memcpy(stack + sp, &gc.heap[reg.l].data, 8);
				sp += 8;
			break;

			case AddI1:
				sp--;
				(*(unsigned char*)(stack + sp-1)) +=
				(*(unsigned char*)(stack + sp));
			break;

			case AddI2:
				sp -= 2;
				(*(unsigned short*)(stack + sp-2)) +=
				(*(unsigned short*)(stack + sp));
			break;

			case AddI4:
				sp -= 4;
				(*(unsigned int*)(stack + sp-4)) +=
				(*(unsigned int*)(stack + sp));
			break;

			case AddI8:
				sp -= 16;
				(*(uint64*)(stack + sp)) +=
				(*(uint64*)(stack + sp+8));
				sp += 8;
			break;

			case AddF4:
				sp -= 4;
				(*(float*)(stack + sp-4)) +=
				(*(float*)(stack + sp));
			break;

			case AddF8:
				sp -= 8;
				(*(double*)(stack + sp-8)) +=
				(*(double*)(stack + sp));
			break;


			case SubI1:
				sp--;
				(*(unsigned char*)(stack + sp-1)) -=
				(*(unsigned char*)(stack + sp));
			break;

			case SubI2:
				sp -= 2;
				(*(unsigned short*)(stack + sp-2)) -=
				(*(unsigned short*)(stack + sp));
			break;

			case SubI4:
				sp -= 4;
				(*(unsigned int*)(stack + sp-4)) -=
				(*(unsigned int*)(stack + sp));
			break;

			case SubI8:
				sp -= 8;
				(*(uint64*)(stack + sp-8)) -= (*(uint64*)(stack + sp));
			break;

			case SubF4:
				sp -= 4;
				(*(float*)(stack + sp-4)) -=
				(*(float*)(stack + sp));
			break;

			case SubF8:
				sp -= 8;
				(*(double*)(stack + sp-8)) -=
				(*(double*)(stack + sp));
			break;


			case MulI1:
				sp--;
				(*(unsigned char*)(stack + sp-1)) *=
				(*(unsigned char*)(stack + sp));
			break;

			case MulI2:
				sp -= 2;
				(*(unsigned short*)(stack + sp-2)) *=
				(*(unsigned short*)(stack + sp));
			break;

			case MulI4:
				sp -= 4;
				(*(unsigned int*)(stack + sp-4)) *=
				(*(unsigned int*)(stack + sp));
			break;

			case MulI8:
				sp -= 8;
				(*(uint64*)(stack + sp-8)) *=
				(*(uint64*)(stack + sp));
			break;

			case MulF4:
				sp -= 4;
				(*(float*)(stack + sp-4)) *=
				(*(float*)(stack + sp));
			break;

			case MulF8:
				sp -= 8;
				(*(double*)(stack + sp-8)) *=
				(*(double*)(stack + sp));
			break;



			case DivI1:
				sp--;
				(*(unsigned char*)(stack + sp-1)) /=
				(*(unsigned char*)(stack + sp));
			break;

			case DivI2:
				sp -= 2;
				(*(unsigned short*)(stack + sp-2)) /=
				(*(unsigned short*)(stack + sp));
			break;

			case DivI4:
				sp -= 4;
				(*(unsigned int*)(stack + sp-4)) /=
				(*(unsigned int*)(stack + sp));
			break;

			case DivI8:
				sp -= 8;
				(*(uint64*)(stack + sp-8)) /=
				(*(uint64*)(stack + sp));
			break;

			case DivF4:
				sp -= 4;
				(*(float*)(stack + sp-4)) /=
				(*(float*)(stack + sp));
			break;

			case DivF8:
				sp -= 8;
				(*(double*)(stack + sp-8)) /=
				(*(double*)(stack + sp));
			break;

			case SmallerThan:
				switch(program[++pc])
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
				*(char*)(stack + (sp++)) = reg.c;
			break;

			case SmallerThanOrEqual:
				switch(program[++pc])
				{
					case INT8:
						sp-=2;
						reg.c = (*(char*)(stack + sp)) <=
						(*(char*)(stack + sp+1));
					break;
					case UINT8:
						sp-=2;
						reg.c = (*(unsigned char*)(stack + sp)) <=
						(*(unsigned char*)(stack + sp+1));
					break;
					case INT16:
						sp -= 4;
						reg.c = (*(short*)(stack + sp)) <=
						(*(short*)(stack + sp+2));
					break;
					case UINT16:
						sp -= 4;
						reg.c = (*(unsigned short*)(stack + sp)) <=
						(*(unsigned short*)(stack + sp+2));
					break;
					case INT32:
						sp -= 8;
						reg.c = (*(int*)(stack + sp)) <=
						(*(int*)(stack + sp+4));
					break;
					case UINT32:
						sp -= 8;
						reg.c = (*(unsigned int*)(stack + sp)) <=
						(*(unsigned int*)(stack + sp+4));
					break;
					case INT64:
						sp -= 16;
						reg.c = (*(int64*)(stack + sp)) <=
						(*(int64*)(stack + sp+8));
					break;
					case UINT64:
					case POINTER:
						sp -= 16;
						reg.c = (*(uint64*)(stack + sp)) <=
						(*(uint64*)(stack + sp+8));
					break;
					case FLOAT32:
						sp -= 8;
						reg.c = (*(float*)(stack + sp)) <=
						(*(float*)(stack + sp+4));
					break;
					case FLOAT64:
						sp -= 16;
						reg.c = (*(double*)(stack + sp)) <=
						(*(double*)(stack + sp+8));
					break;
				}
				stack[sp++] = reg.c;
			break;

			case LargerThan:
				switch(program[++pc])
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
			break;

			case LargerThanOrEqual:
				switch(program[++pc])
				{
					case INT8:
						sp-=2;
						reg.c = (*(char*)(stack + sp)) >=
						(*(char*)(stack + sp+1));
					break;
					case UINT8:
						sp-=2;
						reg.c = (*(unsigned char*)(stack + sp)) >=
						(*(unsigned char*)(stack + sp+1));
					break;
					case INT16:
						sp -= 4;
						reg.c = (*(short*)(stack + sp)) >=
						(*(short*)(stack + sp+2));
					break;
					case UINT16:
						sp -= 4;
						reg.c = (*(unsigned short*)(stack + sp)) >=
						(*(unsigned short*)(stack + sp+2));
					break;
					case INT32:
						sp -= 8;
						reg.c = (*(int*)(stack + sp)) >=
						(*(int*)(stack + sp+4));
					break;
					case UINT32:
						sp -= 8;
						reg.c = (*(unsigned int*)(stack + sp)) >=
						(*(unsigned int*)(stack + sp+4));
					break;
					case INT64:
						sp -= 16;
						reg.c = (*(int64*)(stack + sp)) >=
						(*(int64*)(stack + sp+8));
					break;
					case UINT64:
					case POINTER:
						sp -= 16;
						reg.c = (*(uint64*)(stack + sp)) >=
						(*(uint64*)(stack + sp+8));
					break;
					case FLOAT32:
						sp -= 8;
						reg.c = (*(float*)(stack + sp)) >=
						(*(float*)(stack + sp+4));
					break;
					case FLOAT64:
						sp -= 16;
						reg.c = (*(double*)(stack + sp)) >=
						(*(double*)(stack + sp+8));
					break;
				}
				stack[sp++] = reg.c;
			break;

			case Equal:
				//switch(program[++pc])
				switch(*(program + (++pc)))
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
				//stack[sp++] = reg.c;
				*(char*)(stack + (sp++)) = reg.c;
			break;

			case NotEqual:
				switch(program[++pc])
				{
					case INT8:
						sp-=2;
						reg.c = (*(char*)(stack + sp)) !=
						(*(char*)(stack + sp+1));
					break;
					case UINT8:
						sp-=2;
						reg.c = (*(unsigned char*)(stack + sp)) !=
						(*(unsigned char*)(stack + sp+1));
					break;
					case INT16:
						sp -= 4;
						reg.c = (*(short*)(stack + sp)) !=
						(*(short*)(stack + sp+2));
					break;
					case UINT16:
						sp -= 4;
						reg.c = (*(unsigned short*)(stack + sp)) !=
						(*(unsigned short*)(stack + sp+2));
					break;
					case INT32:
						sp -= 8;
						reg.c = (*(int*)(stack + sp)) !=
						(*(int*)(stack + sp+4));
					break;
					case UINT32:
						sp -= 8;
						reg.c = (*(unsigned int*)(stack + sp)) !=
						(*(unsigned int*)(stack + sp+4));
					break;
					case INT64:
						sp -= 16;
						reg.c = (*(int64*)(stack + sp)) !=
						(*(int64*)(stack + sp+8));
					break;
					case UINT64:
					case POINTER:
						sp -= 16;
						reg.c = (*(uint64*)(stack + sp)) !=
						(*(uint64*)(stack + sp+8));
					break;
					case FLOAT32:
						sp -= 8;
						reg.c = (*(float*)(stack + sp)) !=
						(*(float*)(stack + sp+4));
					break;
					case FLOAT64:
						sp -= 16;
						reg.c = (*(double*)(stack + sp)) !=
						(*(double*)(stack + sp+8));
					break;
				}
				stack[sp++] = reg.c;
			break;

			case If:
				if(stack[--sp])
				{
					pc++;
					pc = *((uint64*)(program + (pc)));
					pc--;
				}
				else pc += 8;
			break;

			case IfNot:
				if(!*(stack + (--sp)))
				{
					pc++;
					pc = *((uint64*)(program + (pc)));
					pc--;
				}
				else pc += 8;
			break;

			case And:
				sp--;
				stack[sp-1] = stack[sp-1] & stack[sp];
			break;

			case Or:
				sp--;
				stack[sp-1] = stack[sp-1] | stack[sp];
			break;

			case Xor:
				sp--;
				stack[sp-1] = stack[sp-1] ^ stack[sp];
			break;

			case Not:
				stack[sp-1] = !stack[sp-1];
			break;
		}
		pc++;
    }
	endLoop:
	printf("%lu\n", (uint64)*(uint64*)(stack));
}

uint64 SilentAlloc(SilentGC* gc, uint64 size)
{
	uint64 returnPos = gc->lastFree;
	gc->heap[gc->lastFree].marked = 0;
	gc->heap[gc->lastFree].occupied = 1;
	gc->heap[gc->lastFree].data = malloc(size);

	for(uint64 i = gc->lastFree + 1; i < gc->heapPtr; i++)
		if(gc->heap[i].occupied == 0) gc->lastFree = i;

	//printf("Allocated %i bytes on location %i\n", size, returnPos);

	return returnPos;
}

void SilentFree(SilentGC* gc, uint64 pos)
{
	gc->heap[pos].occupied = 0;
	free(gc->heap[pos].data);
	if(pos < gc->lastFree) gc->lastFree = pos;

}