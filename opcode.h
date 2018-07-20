
/**
 * This file contains list of opcodes used in SVMv2.
 */

#ifndef OPCODE_H
#define OPCODE_H

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

#endif
