#ifndef ASSEMBLY_H

#define ASSEMBLY_H

#include <stdio.h>
#include "tac.h"

int tacToAssembly(TAC *tac, FILE* output);



#ifdef __linux__

#define MAC_MAIN 0

#define STR_PRINT "\tleaq %s(%%rip), %%rdi\n"\
                 "\tmovl $0, %%eax\n"\
                 "\tcall printf@PLT\n"

#define STR_PRINT_VAR "\tmovl %s(%%rip), %%esi\n"\
                      "\tleaq LCINT(%%rip), %%rdi\n"\
                      "\tmovl $0, %%eax\n"\
                      "\tcall printf@PLT\n"

#define STR_PRINT_NUM "\tmovl $%s, %%esi\n"\
                      "\tleaq LCINT(%%rip), %%rdi\n"\
                      "\tmovl $0, %%eax\n"\
                      "\tcall printf@PLT\n"

#define STR_READ "\tleaq %s(%%rip), %%rsi\n"\
                 "\tleaq LCINT(%%rip), %%rdi\n"\
                 "\tmovl $0, %%eax\n"\
                 "\tcall __isoc99_scanf@PLT\n"

#else
// in this case the target must be __APPLE__

#define MAC_MAIN 1

#define STR_PRINT "\tleaq	%s(%%rip), %%rdi\n"\
				  "\txorl	%%eax, %%eax\n"\
				  "\tmovb	%%al, %%cl\n"\
				  "\tmovl	%%eax, -8(%%rbp)          ## 4-byte Spill\n"\
				  "\tmovb	%%cl, %%al\n"\
				  "\tcallq	_printf\n"\
				  "\tmovl	-8(%%rbp), %%edi          ## 4-byte Reload\n"

#define STR_PRINT_VAR "\tmovl %s(%%rip), %%esi\n"\
					  "\tleaq LCINT(%%rip), %%rdi\n"\
					  "\txorl %%eax, %%eax\n"\
					  "\tmovb %%al, %%cl\n"\
					  "\tmovl %%eax, -8(%%rbp)          ## 4-byte Spill\n"\
					  "\tmovb %%cl, %%al\n"\
					  "\tcallq _printf\n"\
					  "\tmovl -8(%%rbp), %%edi          ## 4-byte Reload\n"\
					  "\tmovl %%eax, -12(%%rbp)         ## 4-byte Spill\n"

#define STR_PRINT_NUM "\tmovl $%s, %%esi\n"\
					  "\tleaq LCINT(%%rip), %%rdi\n"\
					  "\txorl %%eax, %%eax\n"\
					  "\tmovb %%al, %%cl\n"\
					  "\tmovl %%eax, -8(%%rbp)          ## 4-byte Spill\n"\
					  "\tmovb %%cl, %%al\n"\
					  "\tcallq _printf\n"\
					  "\tmovl -8(%%rbp), %%edi          ## 4-byte Reload\n"\
					  "\tmovl %%eax, -12(%%rbp)         ## 4-byte Spill\n"

#define STR_READ "\tleaq LCINT(%%rip), %%rdi\n"\
				 "\tmovq %s@GOTPCREL(%%rip), %%rsi\n"\
				 "\txorl %%eax, %%eax\n"\
				 "\tmovb %%al, %%cl\n"\
				 "\tmovl %%eax, -8(%%rbp)          ## 4-byte Spill\n"\
				 "\tmovb %%cl, %%al\n"\
				 "\tcallq _scanf\n"\
				 "\tmovl -8(%%rbp), %%edi          ## 4-byte Reload\n"\
				 "\tmovl %%eax, -12(%%rbp)         ## 4-byte Spill\n"
#endif




#endif
