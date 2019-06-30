#ifndef ASSEMBLY_H

#define ASSEMBLY_H

#include <stdio.h>
#include "tac.h"

int tacToAssembly(TAC *tac, FILE* output);



#ifdef __linux__

#define STR_PUTS "\tleaq %s(%%rip), %%rdi\n"\
                 "\tcall puts@PLT\n"

#else
// in this case the target must be __APPLE__
#define STR_PUTS "\tleaq	%s(%rip), %rdi\n"\
	               "\txorl	%eax, %eax\n"\
	               "\tmovb	%al, %cl\n"\
	               "\tmovl	%eax, -8(%rbp)          ## 4-byte Spill\n"\
	               "\tmovb	%cl, %al\n"\
	               "\tcallq	_printf\n"\
                 "\tmovl	-8(%rbp), %edi          ## 4-byte Reload\n"


#endif



#endif
