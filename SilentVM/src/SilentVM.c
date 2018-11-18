#include "SilentVM.h"

typedef unsigned long long uint64;
typedef unsigned long long int64;
void SilentStartVM(char* program)
{
    char* stack = malloc(10000);
    unsigned long long sp = 0;
    unsigned long long fp = 0;
    unsigned long long pc = 0;

    unsigned long long *saveSfData = malloc(90000); //old stack frame data
    unsigned long long saveSfDataPtr = 0;


    union Registers
	{
		char c;
		short s;
		int i;
		long long l;
		float f;
		double d;
	}reg, reg2, reg3, reg4;

    char running = 1;

    while(running)
    {
        
        switch (program[pc])
        {
            case Halt:
                running = 0;
            break;
        
            case Goto:
                pc = *((uint64*)(program + (pc+1)));
                pc--;
            break;

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

            case Push1:
                stack[sp++] = program[++pc];
				//printf("%i\n", stack[sp-1]);
            break;

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
				memcpy(stack + sp, program + (++pc), 8);
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
				memcpy(stack + sp, stack + fp + reg.l, 1);
				sp++;
			break;

			case Load2:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 2);
				sp += 2;
			break;

			case Load4:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 4);
				sp += 4;
			break;

			case Load8:
				//memcpy(&reg.l, program + ++pc, 8);
                reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + fp + reg.l, 9);
				sp += 8;
			break;

			case StoreGlobal1:
				//Get global offset
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 1;
				memcpy(stack + reg.l, stack + sp, 1);
			break;
			
			case StoreGlobal2:
				//Get global offset
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 2;
				memcpy(stack + reg.l, stack + sp, 2);
				//SilentPopBack(stackT);
				//stackTPtr--;
			break;

			case StoreGlobal4:
				//Get global offset
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 4;
				memcpy(stack + reg.l, stack + sp, 4);
				//SilentPopBack(stackT);
				//stackTPtr--;
			break;

			case StoreGlobal8:
				//Get global offset
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				//Store
				sp -= 8;
				memcpy(stack + reg.l, stack + sp, 8);
				//SilentPopBack(stackT);
				//stackTPtr--;
			break;

			case LoadGlobal1:
				//SilentPushBack(stackT, dt + program[++pc]);
				//stackT[stackTPtr++] = program[++pc];
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 1);
				sp++;
			break;

			case LoadGlobal2:
				//SilentPushBack(stackT, dt + program[++pc]);
				//stackT[stackTPtr++] = program[++pc];
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 2);
				sp += 2;
			break;

			case LoadGlobal4:
				//SilentPushBack(stackT, dt + program[++pc]);
				//stackT[stackTPtr++] = program[++pc];
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 4);
				sp += 4;
			break;

			case LoadGlobal8:
				//SilentPushBack(stackT, dt + program[++pc]);
				//stackT[stackTPtr++] = program[++pc];
				//memcpy(&reg.l, program + ++pc, 8);
				reg.l = (uint64)*((uint64*)(program + (++pc)));
				pc += 7;
				memcpy(stack + sp, stack + reg.l, 8);
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
				sp -= 8;
				(*(uint64*)(stack + sp-8)) +=
				(*(uint64*)(stack + sp));
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
				(*(uint64*)(stack + sp-8)) -=
				(*(uint64*)(stack + sp));
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

			case Equal:
				switch(program[++pc])
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
				if(!stack[--sp])
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

			case Not:
				stack[sp-1] = !stack[sp-1];
			break;
		}
		pc++;
    }
	printf("%ul\n", (uint64)*(uint64*)(stack));
}