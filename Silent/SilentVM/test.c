#include "SilentVM.h"

#define BYTECODE_HALT 0
#define BYTECODE_GOTO 1
#define BYTECODE_CALL 2

#define BYTECODE_CLEAR_MEMORY 3
#define BYTECODE_CLEAR_STACK 4

#define BYTECODE_PUSH_BYTE 5
#define BYTECODE_PUSH_INT 6
#define BYTECODE_PUSH_FLOAT 7

#define BYTECODE_POP_BYTE 8
#define BYTECODE_POP4 9

#define BYTECODE_STORE_BYTE 10
#define BYTECODE_STORE_INT 11
#define BYTECODE_STORE_FLOAT 12

#define BYTECODE_LOAD_BYTE 13
#define BYTECODE_LOAD_INT 14
#define BYTECODE_LOAD_FLOAT 15

#define BYTECODE_ADD_BYTE 16
#define BYTECODE_ADD_INT 17
#define BYTECODE_ADD_FLOAT 18

#define BYTECODE_SUB_BYTE 19
#define BYTECODE_SUB_INT 20
#define BYTECODE_SUB_FLOAT 21

#define BYTECODE_MUL_BYTE 22
#define BYTECODE_MUL_INT 23
#define BYTECODE_MUL_FLOAT 24

#define BYTECODE_DIV_BYTE 25
#define BYTECODE_DIV_INT 26
#define BYTECODE_DIV_FLOAT 27

#define BYTECODE_BYTE_TO_INT 28
#define BYTECODE_INT_TO_BYTE 29
#define BYTECODE_INT_TO_FLOAT 30
#define BYTECODE_FLOAT_TO_INT 31

#define BYTECODE_SMALLER_THAN 32
#define BYTECODE_BIGGER_THAN 33
#define BYTECODE_EQUAL 34

#define BYTECODE_IF 35
#define BYTECODE_IF_NOT 36

int main() 
{

	SilentVM* vm;
	SilentThread* thread;
	SilentMemory* memory;

	char* bytecode = malloc(100);
	int x = 0;
	bytecode[x++] = BYTECODE_PUSH_BYTE;
	bytecode[x++] = 5;
	bytecode[x++] = BYTECODE_PUSH_BYTE;
	bytecode[x++] = 2;
	bytecode[x++] = BYTECODE_ADD_BYTE;
	//bytecode[x++] = BYTECODE_CLEAR_STACK;
	bytecode[x++] = BYTECODE_HALT;


	memory = createSilentMemory(500, 500);
	thread = createSilentThread(bytecode, memory);
	vm = createSilentVM(thread, 1);
	loadProgramToSilentVM(vm, bytecode);

	startSilentVM(vm);
	printf("%i", memory->stack[0]);
	getchar();
	return 0;
}