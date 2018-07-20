#include "vm.h"
#include "opcode.h"

#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE 1024
#define PUSH(vm, v) (vm->stack[++vm->opsp] = (bytecode_t)(v))
#define POP(vm) (vm->stack[vm->opsp--])
#define PUSHARG(vm, v) (vm->locals[vm->sp++] = v)
#define NEXTCODE(vm) (vm->code[vm->pc++])
#define MAX(a,b) (((a)>(b))?(a):(b))

struct VirtualMachine {
	int32_t loc_size;

	bytecode_t * locals;
	bytecode_t * code;
	bytecode_t * stack;

	int32_t opsp, pc, sp, fp;
};

VirtualMachine* create_vm(code, pc, datasize) bytecode_t *code; int32_t pc; int32_t datasize; {
	VirtualMachine *vm = malloc(sizeof(*vm));
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

void delete_vm(vm) VirtualMachine *vm; {
	free(vm->stack);
	free(vm->locals);
	free(vm);
}

void exec(vm) VirtualMachine *vm; {
    bytecode_t a, b, v, addr, offset, argc;
    
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
			printf("%d\n", v.POP(vm));
			continue;
		case PRINT_FP:
			printf("%f\n", v.POP(vm));
			continue;
		default:
			continue;
		}
}

#undef MAX
