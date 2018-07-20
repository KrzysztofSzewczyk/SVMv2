
/**
 * MAIN.C
 * 
 * This file is setting up virtual machine with data read from file passed as parameter.
 */

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "opcode.h"
#include "endian.h"

static void cthbo(bytecode, length) bytecode_t *bytecode; size_t length; {
    size_t i = 0;
    
	for(; i < length; i++)
		bytecode[i].int32 = be32toh(bytecode[i].int32);
}

main(argc, argv) int argc; char * argv[]; {
    VirtualMachine *vm;
    long size;
    FILE * program;
    bytecode_t * bytecode;
    
	if(argc < 2) {
		fprintf(stderr, "Error: No binary file passed as parameter.");
		exit(1);
	}
	
	program = fopen(argv[1], "r+b");
	if(program == NULL) {
		fprintf(stderr, "Error: Could not open provided file.\n");
		exit(1);
	}
    
	fseek(program, 0, SEEK_END);
	size = ftell(program) / sizeof(bytecode_t);
	rewind(program);

	bytecode = malloc(sizeof(bytecode_t) * size);
	fread(bytecode, size, sizeof(bytecode_t), program);

	fclose(program);

	cthbo(bytecode, size);

	vm = create_vm(bytecode, 0, 4096 * 1024);
	exec(vm);
	delete_vm(vm);

	free(bytecode);
}
