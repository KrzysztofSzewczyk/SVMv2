
/**
 * VM.C
 * 
 * This file contains implementation for VM.H declared prototypes.
 */

#include "vm.h"

#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE 1024
#define PUSH(vm, v) vm->stack[++vm->opsp] = (bytecode_t)(v)
#define POP(vm) vm->stack[vm->opsp--]
#define PUSHARG(vm, v) vm->locals[vm->sp++] = v
#define NEXTCODE(vm) vm->code[vm->pc++]
#define MAX(a,b) (((a)>(b))?(a):(b))

enum Opcode {
	HALT = 0,           /* Halt the MV */
	ADD_I32 = 1,        /* Add two 32bit numbers */
	SUB_I32 = 2,        /* Subtract two 32bit numbers */
	MUL_I32 = 3,        /* Multiply two 32bit numbers */
	DIV_I32 = 4,        /* Divide two 32bit numbers */
	MOD_I32 = 5,        /* Execute modulus operation two 32bit numbers */
	LT_I32 = 6,         /* Less-than like operation on two 32bit numbers */
	LE_I32 = 7,         /* Less-than or equal like operation on two 32bit numbers */
	GT_I32 = 8,         /* Greater-than like operation on two 32bit numbers */
	GE_I32 = 9,         /* Greater-than or equal like operation on two 32bit numbers */
	EQ_I32 = 10,        /* Equal like operation on two 32bit numbers */
	NEQ_I32 = 11,       /* Not equal like operation on two 32bit numbers */
	JMP = 12,           /* Unconditional branching */
	JMPT = 13,          /* Branch if 6-11 are true */
	JMPF = 14,          /* Branch if 6-11 are false */
	CONST_I32 = 15,     /* Define 32bit immediate value */
	LOAD = 16,          /* Load local and push it on stack. */
	STORE = 17,         /* Store local popping its value from stack. */
	PUSHARG = 18,       /* Push function parameter on stack. To be used with CALL */
	PRINT = 19,         /* Print topmost value from stack. */
	POP = 20,           /* Discard topmost on stack. */
	CALL = 21,          /* Call function using created new stack frame. */
	GETARG = 22,        /* Get parameter set with PUSHARG */
	RET = 23,           /* Return from function restoring old frame */
	RETVOID = 24,       /* Return without passing value. */
	DUP = 25,           /* Duplicate topmost value on stack. */
	CONST_F32 = 26,     /* Define floating point constant. */
	PRINT_FP = 27,      /* Print topmost value on stack as floating point number. */
};

struct vm_t {
	int32_t loc_size;

	bytecode_t * locals;
	bytecode_t * code;
	bytecode_t * stack;

	int32_t opsp, pc, sp, fp;
};

vm_t* creatvm(code, pc, datasize) bytecode_t *code; int32_t pc; int32_t datasize; {
	vm_t *vm = malloc(sizeof(*vm));
	vm->code = code;
	vm->pc = pc;
	vm->opsp = -1;
	vm->fp = 0;
	vm->sp = 0;
	vm->loc_size = datasize;
	vm->locals = malloc(sizeof(bytecode_t) * datasize);
	vm->stack = malloc(sizeof(bytecode_t) * STACK_SIZE);
	return vm;
}

void delvm(vm) vm_t *vm; {
	free(vm->stack);
	free(vm->locals);
	free(vm);
}

void exec(vm) vm_t *vm; {
    bytecode_t v, addr, offset, argc;
    
	for(;;)
		switch (NEXTCODE(vm).int32) {
		case HALT: return;
		case CONST_I32:
			PUSH(vm, NEXTCODE(vm));
			continue;
		case ADD_I32:
			PUSH(vm, POP(vm).int32 + POP(vm).int32);
			continue;
		case SUB_I32:
			PUSH(vm, POP(vm).int32 - POP(vm).int32);
			continue;
		case MUL_I32:
			PUSH(vm, POP(vm).int32 * POP(vm).int32);
			continue;
		case DIV_I32:
			PUSH(vm, POP(vm).int32 / POP(vm).int32);
			continue;
		case MOD_I32:
			PUSH(vm, POP(vm).int32 % POP(vm).int32);
			continue;
		case LT_I32:
			PUSH(vm, (POP(vm).int32 < POP(vm).int32) ? 1 : 0);
			continue;
		case LE_I32:
			PUSH(vm, (POP(vm).int32 <= POP(vm).int32) ? 1 : 0);
			continue;
		case GT_I32:
			PUSH(vm, (POP(vm).int32 > POP(vm).int32) ? 1 : 0);
			continue;
		case GE_I32:
			PUSH(vm, (POP(vm).int32 >= POP(vm).int32) ? 1 : 0);
			continue;
		case EQ_I32:
			PUSH(vm, (POP(vm).int32 == POP(vm).int32) ? 1 : 0);
			continue;
		case NEQ_I32:
			PUSH(vm, (POP(vm).int32 != POP(vm).int32) ? 1 : 0);
			continue;
		case CONST_F32:
			PUSH(vm, NEXTCODE(vm));
			continue;
		case DUP:
			PUSH(vm, vm->stack[vm->sp]);
			continue;
		case JMP:
			vm->pc = NEXTCODE(vm).int32;
			continue;
		case JMPT:
			addr = NEXTCODE(vm);
			if(POP(vm).int32)
				vm->pc = addr.int32;
			continue;
		case JMPF:
			addr = NEXTCODE(vm);
			if(!POP(vm).int32)
				vm->pc = addr.int32;
			continue;
		case LOAD:
			PUSH(vm, vm->locals[vm->fp + NEXTCODE(vm).int32]);
			continue;
		case STORE:
			v = POP(vm);
			offset = NEXTCODE(vm);
			vm->locals[vm->fp + offset.int32] = v;
			vm->sp = MAX(vm->sp, vm->fp + offset.int32 + 1);
			continue;
		case PUSHARG:
			PUSHARG(vm, POP(vm));
			continue;
		case CALL:
			addr = NEXTCODE(vm);
			argc = NEXTCODE(vm);
			PUSH(vm, vm->pc);
			PUSH(vm, vm->fp);
			vm->fp = vm->sp - argc.int32;
			vm->pc = addr.int32;
			continue;
		case RET:
			v = POP(vm);
			vm->sp = vm->fp;
			vm->fp = POP(vm).int32;
			vm->pc = POP(vm).int32;
			PUSH(vm, v);
			continue;
		case RETVOID:
			vm->sp = vm->fp;
			vm->fp = POP(vm).int32;
			vm->pc = POP(vm).int32;
			continue;
		case POP:
			POP(vm);
			continue;
		case PRINT:
			printf("%d\n", POP(vm).int32);
			continue;
		case PRINT_FP:
			printf("%f\n", POP(vm).fp32);
			continue;
		default:
			continue;
		}
}
