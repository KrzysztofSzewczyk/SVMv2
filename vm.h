
/**
 * VM.H
 *
 * This file is corresponding header to VM.C
 */

#ifndef VM_H
#define VM_H

#include <stdlib.h>
#include <stdint.h>

typedef union bytecode_t {
	int32_t int32;
	float   fp32;
} bytecode_t;
typedef struct vm_t vm_t;

vm_t* creatvm(bytecode_t *code, int32_t pc, int32_t datasize);
void deletvm(vm_t *vm);
void exec(vm_t *vm);

#endif
