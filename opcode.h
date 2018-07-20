#ifndef OPCODE_H
#define OPCODE_H

enum Opcode {
	HALT = 0,
	ADD_I32 = 1,
	SUB_I32 = 2,
	MUL_I32 = 3,
	DIV_I32 = 4,
	MOD_I32 = 5,
	LT_I32 = 6,
	LE_I32 = 7,
	GT_I32 = 8,
	GE_I32 = 9,
	EQ_I32 = 10,
	NEQ_I32 = 11,
	JMP = 12,
	JMPT = 13,
	JMPF = 14,
	CONST_I32 = 15,
	LOAD = 16,
	STORE = 17,
	PUSHARG = 18,
	PRINT = 19,
	POP = 20,
	CALL = 21,
	GETARG = 22,
	RET = 23,
	RETVOID = 24,
	DUP = 25,
	CONST_F32 = 26,
	PRINT_FP = 27,
};

#endif
