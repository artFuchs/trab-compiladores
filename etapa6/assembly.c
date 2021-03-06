#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembly.h"
#include "hash.h"
#include "symbols.h"

void addMacMain();
void declareStrings(FILE *output);
void fixIntVarNames();
char* leapNumToDecNum(char *num);
int createAssembly(TAC *tac, FILE *output);
void createVARDECL(TAC *tac, FILE *output);
void createARRAYDECL(TAC *tac, FILE *output);
void createINITARRAY(TAC *tac, FILE *output, int type);
void createBEGFUN(TAC *tac, FILE *output);
void createENDFUN(TAC *tac, FILE *output);
void createRETURN(TAC *tac, FILE *output);
void createPRINT(TAC *tac, FILE *output);
void createMOVE(TAC *tac, FILE *output);
void createARRAYW(TAC *tac, FILE *output);
void createARRAYR(TAC *tac, FILE *output);
void createBinop(int op, TAC *tac, FILE *output);
void createDIV(TAC* tac, FILE* output);
void createIFZ(TAC* tac, FILE* output);
void createCALL(TAC* tac, FILE* output);
void createREAD(TAC* tac, FILE *output);

int tacToAssembly(TAC* tac, FILE* output){
  if (!tac) return 1;

  addMacMain();
  declareStrings(output);
  fixIntVarNames();

  if (createAssembly(tac, output)) return 2;

  return 0;
}

void addMacMain(){
  if (!MAC_MAIN) return;
  int i;
  for (i=0; i<HASH_SIZE; i++){
    NODE* node = Table[i];
    while (node){
      if (node->type == SYMBOL_FUNC && !strcmp(node->text,"main")){
        strcpy(node->text,"_main");
      }
      node = node->next;
    }
  }
}


void declareStrings(FILE *output){
  int i=0;
  static int lbl_num = 0;
  fprintf(output,
          "\t.text\n"
          "LCINT:\n"
          "\t.string \"%%d\"\n");

  fprintf(output,
          "\t.text\n"
          "LCFLOAT:\n"
          "\t.string \"%%f\"\n");

  for (i=0; i<HASH_SIZE; i++){
    if (Table[i]){
      NODE* node = Table[i];
      while (node){
        if (node->type == SYMBOL_LIT_STRING){
          char lbl_str [24];
          sprintf(lbl_str, "LC%d",lbl_num++);
          fprintf(output,
                 "%s:\n"
                 "\t.string \"%s\"\n",
                 lbl_str,
                 node->text);
          strcpy(node->text, lbl_str);
        }
        node = node->next;
      }
    }
  }
}

void fixIntVarNames(){
  int i=0;
  for (i=0; i<HASH_SIZE; i++){
    if (Table[i]){
      NODE* node = Table[i];
      while (node){
        if (node->type == SYMBOL_VAR || node->type == SYMBOL_ARRAY){
          if (isdigit(node->text[0])){
            char *p;
            int ret = strtol(node->text,&p,10);
            if (*p==0){
              sprintf(node->text,"_NuMvAr_%d", ret);
            }

          }
        }
        node = node->next;
      }
    }
  }
}

char* leapNumToDecNum(char *num){
  int len = strlen(num);
  if (len==0) return 0;
  char* buffer = (char*) calloc (len,sizeof(char));
  int i=0;
  for (i=0; i<len; i++){
    switch(num[i]){
      case 'O': buffer[i] = '0'; break;
      case 'I': buffer[i] = '1'; break;
      case 'H': buffer[i] = '2'; break;
      case 'G': buffer[i] = '3'; break;
      case 'F': buffer[i] = '4'; break;
      case 'E': buffer[i] = '5'; break;
      case 'D': buffer[i] = '6'; break;
      case 'C': buffer[i] = '7'; break;
      case 'B': buffer[i] = '8'; break;
      case 'A': buffer[i] = '9'; break;
      case '.': buffer[i] = '.'; break;
      default: buffer[i] = 0;
    }
  }
  return buffer;
}

int createAssembly(TAC *tac, FILE *output){
  if (!tac) return 0;
  static int last_array_type = 0;
  switch (tac->instruction){
    case TAC_BYTE_DECL:
    case TAC_INT_DECL:
    case TAC_FLOAT_DECL: createVARDECL(tac, output); break;
    case TAC_INTA_DECL:
    case TAC_BYTEA_DECL:
    case TAC_FLOATA_DECL:
      createARRAYDECL(tac, output);
      last_array_type = tac->instruction;
      break;
    case TAC_ARRAY_INIT: createINITARRAY(tac, output, last_array_type); break;
    case TAC_BEGINFUN: createBEGFUN(tac, output); break;
    case TAC_RETURN: createRETURN(tac, output); break;
    case TAC_ENDFUN: createENDFUN(tac, output); break;
    case TAC_PRINT: createPRINT(tac, output); break;
    case TAC_MOVE: createMOVE(tac, output); break;
    case TAC_ARRAYW: createARRAYW(tac, output); break;
    case TAC_ARRAYR: createARRAYR(tac, output); break;
    case TAC_ADD:
    case TAC_SUB:
    case TAC_MUL:
    case TAC_LT:
    case TAC_GT:
    case TAC_LE:
    case TAC_GE:
    case TAC_EQ:
    case TAC_NEQ:
      createBinop(tac->instruction, tac, output); break;
    case TAC_DIV: createDIV(tac,output); break;
    case TAC_IFZ: createIFZ(tac, output); break;
    case TAC_LABEL: fprintf(output, "%s:\n", tac->result->text); break;
    case TAC_JUMP: fprintf(output, "\tjmp %s\n", tac->result->text); break;
    case TAC_CALL: createCALL(tac,output); break;
    case TAC_ARGWRITE: createMOVE(tac,output); break;
    case TAC_READ: createREAD(tac,output);

    default:  break;
  }

  createAssembly(tac->next, output);
  return 0;
}

void createVARDECL(TAC *tac, FILE *output){
  int size=4;
  char* varName;
  char zeros[8];
  char val[28] = "0";
  varName = tac->result->text;
  sprintf (zeros,".zero %d", size);
  if (tac->op1){
    char *num = leapNumToDecNum(tac->op1->text);
    switch (tac->instruction){
      case TAC_INT_DECL:
      case TAC_BYTE_DECL:
        sprintf (val, ".long %s", num);
        break;
      case TAC_FLOAT_DECL:
        sprintf (val, ".float %s", num);
        break;
    }
    free(num);
  }
  fprintf(output,
          "\t.data\n"
          "\t.globl %s\n"
          "\t.align 4\n"
          "%s:\n"
          "\t%s\n",
          varName, varName, tac->op1?val:zeros);
}

void createARRAYDECL(TAC *tac, FILE *output){
  char* varName = tac->result->text;
  fprintf(output,
          "\t.data\n"
          "\t.globl %s\n"
          "%s:\n",
          varName, varName);
  if (tac->next->instruction == TAC_ARRAY_INIT) return;
  char* num = leapNumToDecNum(tac->op1->text);
  int arraySize = strtol(num,NULL,10);
  free(num);
  int i;
  for (i=0;i<arraySize;i++)
    fprintf(output, "\t.long 0\n");
}

void createINITARRAY(TAC* tac, FILE *output, int type){
  char* num = leapNumToDecNum(tac->op1->text);
  switch (type){
    case TAC_BYTEA_DECL:
    case TAC_INTA_DECL:
      fprintf(output, "\t.long %s\n", num);
      break;
    case TAC_FLOAT_DECL:
      fprintf(output, "\t.float %s\n", num);
      break;
  }
  free(num);
}

void createBEGFUN(TAC *tac, FILE *output){
  fprintf(output,
          "\t.text \n"
          "\t.globl	%s\n"
          "%s:\n"
          "\tpushq	%%rbp\n"
          "\tmovq	%%rsp, %%rbp \n",
          tac->result->text,
          tac->result->text);
}

void createENDFUN(TAC *tac, FILE *output){
  fprintf(output,
          "\tpopq	%%rbp\n"
          "\tret \n");
}

void createRETURN(TAC *tac, FILE *output){
  char *num = NULL;
  switch (tac->result->type){
      case SYMBOL_LIT_INT:
      case SYMBOL_LIT_BYTE:
        num = leapNumToDecNum(tac->result->text);
        fprintf(output,
                "movl	$%s, %%eax \n",
                num);
        break;
      case SYMBOL_VAR:
        if (!strstr(tac->result->text,"__tempvar")){
          fprintf(output,
                  "movl	%s(%%rip), %%eax \n",
                  tac->result->text);
        }
        break;
  }
  free(num);
}

void createPRINT(TAC *tac, FILE *output){
  char* num = NULL;
  switch (tac->result->type){
    case SYMBOL_LIT_STRING:
      fprintf(output,
              STR_PRINT,
              tac->result->text);
      break;
    case SYMBOL_LIT_INT:
    case SYMBOL_LIT_BYTE:
      num = leapNumToDecNum(tac->result->text);
      fprintf(output,
              STR_PRINT_NUM,
              num);
      break;
    case SYMBOL_VAR:
      if (strstr(tac->result->text,"__tempvar")){
        fprintf(output,"\tmovl %%eax, %%esi\n");
        fprintf(output,
                STR_PRINT,
                "LCINT");
      }
      else{
        fprintf(output,
                STR_PRINT_VAR,
                tac->result->text);
      }
      break;
  }
  free(num);
}

void createMOVE(TAC *tac, FILE *output){
  char* num = NULL;
  switch (tac->op1->type){
    case SYMBOL_LIT_INT:
    case SYMBOL_LIT_BYTE:
      num = leapNumToDecNum(tac->op1->text);
      fprintf(output,"\tmovl $%s, %s(%%rip)\n", num, tac->result->text);
      free(num);
      break;
    case SYMBOL_LIT_FLOAT:
      break;
    case SYMBOL_VAR:
      if (strstr(tac->op1->text, "__tempvar")){
        fprintf(output,
                "\tmovl %%eax, %s(%%rip)\n",
                tac->result->text);
      }else{
        fprintf(output,
                "\tmovl %s(%%rip), %%eax\n"
                "\tmovl %%eax, %s(%%rip)\n"
                ,tac->op1->text, tac->result->text);
      }
      break;
  }
}

void createARRAYW(TAC *tac, FILE *output){
  //op1 = array index
  //op2 = data to write

  int fstvar = tac->op1->type == SYMBOL_VAR;
  int sndvar = tac->op2->type == SYMBOL_VAR;
  int fstint = tac->op1->type == SYMBOL_LIT_INT || tac->op1->type == SYMBOL_LIT_BYTE;
  int sndint = tac->op2->type == SYMBOL_LIT_INT || tac->op2->type == SYMBOL_LIT_BYTE;
  if (fstvar && sndvar ){
    fprintf(output,"\tmovl	%s(%%rip), %%eax\n"
  	               "\tmovl	%s(%%rip), %%edx\n"
  	               "\tcltq\n"
  	               "\tleaq	0(,%%rax,4), %%rcx\n"
  	               "\tleaq	%s(%%rip), %%rax\n"
  	               "\tmovl	%%edx, (%%rcx,%%rax)\n",
                  tac->op1->text, tac->op2->text, tac->result->text);

  }else if (fstvar && sndint){
    char *num = leapNumToDecNum(tac->op2->text);
    fprintf(output, "\tmovl %s(%%rip), %%eax\n"
                    "\tcltq\n"
                    "\tleaq 0(,%%rax,4), %%rdx\n"
                    "\tleaq %s(%%rip), %%rax\n"
                    "\tmovl $%s, (%%rdx,%%rax)\n",
                    tac->op1->text, tac->result->text, num);
    free(num);
  }else if (fstint && sndvar){
    char *pos = leapNumToDecNum(tac->op1->text);
    int position = 4 * strtol(pos,NULL,10);
    fprintf(output, "\tmovl	%s(%%rip), %%eax\n"
  	                "\tmovl	%%eax, %d+a(%%rip)\n",
                   tac->op2->text, position);
    free(pos);
  }else if (fstint && sndint){
    char *pos = leapNumToDecNum(tac->op1->text);
    int position = 4 * strtol(pos,NULL,10);
    char *data = leapNumToDecNum(tac->op2->text);
    fprintf(output, "\tmovl	$%s, %d+a(%%rip)\n",
                  data, position);
    free(pos);
    free(data);
  }
}

void createARRAYR(TAC *tac, FILE *output){
  char *num=0;
  switch (tac->op2->type){
    case SYMBOL_LIT_INT:
      num = leapNumToDecNum(tac->op2->text);
      fprintf(output, "\tmovl %s+%s(%%rip), %%eax\n", num, tac->op1->text);
      break;
    case SYMBOL_VAR:
      if (!strstr(tac->op2->text, "__tempvar")){
        fprintf(output, "\tmovl %s(%%rip), %%eax\n"
                        "\tcltq\n"
                        "\tleaq 0(,%%rax,4), %%rdx\n"
                        "\tleaq %s(%%rip), %%rax\n"
                        "\tmovl (%%rdx,%%rax), %%eax\n",
                      tac->op2->text, tac->op1->text);
      }
  }
  free(num);
}

void createBinopVar(int op, FILE *output){
  switch (op){
    case TAC_ADD:
      fprintf(output, "\taddl %%edx, %%eax\n");
      break;
    case TAC_SUB:
      fprintf(output, "\tsubl %%eax, %%edx\n"
                      "\tmovl %%edx, %%eax\n");
      break;
    case TAC_MUL:
      fprintf(output, "\timul %%edx, %%eax\n");
      break;
    case TAC_GT:
      fprintf(output, "\tcmpl %%eax, %%edx\n"
                      "\tsetg %%al\n"
                      "\tmovzbl %%al, %%eax\n");
      break;
    case TAC_LT:
      fprintf(output, "\tcmpl %%eax, %%edx\n"
                      "\tsetl %%al\n"
                      "\tmovzbl %%al, %%eax\n");
      break;
    case TAC_GE:
      fprintf(output, "\tcmpl %%eax, %%edx\n"
                      "\tsetge %%al\n"
                      "\tmovzbl %%al, %%eax\n");
      break;
    case TAC_LE:
      fprintf(output, "\tcmpl %%eax, %%edx\n"
                      "\tsetle %%al\n"
                      "\tmovzbl %%al, %%eax\n");
      break;
    case TAC_EQ:
      fprintf(output, "\tcmpl %%eax, %%edx\n"
                      "\tsete %%al\n"
                      "\tmovzbl %%al, %%eax\n");
      break;
    case TAC_NEQ:
      fprintf(output, "\tcmpl %%eax, %%edx\n"
                      "\tsetne %%al\n"
                      "\tmovzbl %%al, %%eax\n");
      break;
  }
}

void createBinopNum(int op, char* num, FILE *output){
  switch (op){
    case TAC_ADD:
      fprintf(output, "\taddl $%s, %%eax\n", num);
      break;
    case TAC_SUB:
      fprintf(output, "\tsubl $%s, %%eax\n", num);
      break;
    case TAC_MUL:
      fprintf(output, "\timul $%s, %%eax\n", num);
      break;
    case TAC_GT:
      fprintf(output, "\tmovl $%s, %%edx\n"
                      "\tcmpl %%edx, %%eax\n"
                      "\tsetg %%al\n"
                      "\tmovzbl %%al, %%eax\n", num);
      break;
    case TAC_LT:
      fprintf(output, "\tmovl $%s, %%edx\n"
                      "\tcmpl %%edx, %%eax\n"
                      "\tsetl %%al\n"
                      "\tmovzbl %%al, %%eax\n", num);
      break;
    case TAC_GE:
      fprintf(output, "\tmovl $%s, %%edx\n"
                      "\tcmpl %%edx, %%eax\n"
                      "\tsetge %%al\n"
                      "\tmovzbl %%al, %%eax\n", num);
      break;
    case TAC_LE:
      fprintf(output, "\tmovl $%s, %%edx\n"
                      "\tcmpl %%edx, %%eax\n"
                      "\tsetle %%al\n"
                      "\tmovzbl %%al, %%eax\n", num);
      break;
    case TAC_EQ:
      fprintf(output, "\tmovl $%s, %%edx\n"
                      "\tcmpl %%edx, %%eax\n"
                      "\tsete %%al\n"
                      "\tmovzbl %%al, %%eax\n", num);
      break;
    case TAC_NEQ:
      fprintf(output, "\tmovl $%s, %%edx\n"
                      "\tcmpl %%edx, %%eax\n"
                      "\tsetne %%al\n"
                      "\tmovzbl %%al, %%eax\n", num);
      break;
  }
}

void createBinop(int op, TAC *tac, FILE *output){
  int fvar = tac->op1->type == SYMBOL_VAR;
  int svar = tac->op2->type == SYMBOL_VAR;
  int dvars = fvar && svar;
  if (dvars){

    if (strstr(tac->op1->text, "__tempvar")){
      fprintf(output, "\tmovl %s(%%rip), %%edx\n", tac->op2->text);
    }
    else if (strstr(tac->op2->text, "__tempvar")){
      fprintf(output, "\tmovl %s(%%rip), %%edx\n", tac->op1->text);
    }else{
      fprintf(output, "\tmovl %s(%%rip), %%edx\n"
                      "\tmovl %s(%%rip), %%eax\n",
                      tac->op1->text, tac->op2->text);
    }
    createBinopVar(op, output);

  }else if (fvar){
    char *num = NULL;
    switch(tac->op2->type){
      case SYMBOL_LIT_BYTE:
      case SYMBOL_LIT_INT:
        num = leapNumToDecNum(tac->op2->text);
        if (!strstr(tac->op1->text, "__tempvar"))
          fprintf(output, "\tmovl %s(%%rip), %%eax\n", tac->op1->text);
        createBinopNum(op, num, output);

    }
    free(num);
  }else if (svar){
    char *num = NULL;
    switch(tac->op1->type){
      case SYMBOL_LIT_BYTE:
      case SYMBOL_LIT_INT:
        num = leapNumToDecNum(tac->op1->text);
        if (!strstr(tac->op2->text, "__tempvar"))
          fprintf(output, "\tmovl %s(%%rip), %%eax\n", tac->op2->text);
        createBinopNum(op, num, output);
    }
    free(num);
  }else{
      char *num1 = NULL;
      char *num2 = NULL;
      if (tac->op1->type == SYMBOL_LIT_INT || tac->op1->type == SYMBOL_LIT_BYTE)
        num1 = leapNumToDecNum(tac->op1->text);
      else
        return;

      if (tac->op2->type == SYMBOL_LIT_INT || tac->op2->type == SYMBOL_LIT_BYTE)
        num2 = leapNumToDecNum(tac->op2->text);
      else{
        free(num1);
        return;
      }

      fprintf(output, "\tmovl $%s, %%eax\n", num1);
      createBinopNum(op,num2,output);

      free(num1);
      free(num2);
    }
}

void createDIV(TAC* tac, FILE* output){
  int fvar = tac->op1->type == SYMBOL_VAR;
  int svar = tac->op2->type == SYMBOL_VAR;
  char *num1 = NULL;
  char *num2 = NULL;

  if (fvar && !strstr(tac->op1->text, "__tempvar")){
    fprintf(output, "\tmovl %s(%%rip), %%eax\n", tac->op1->text);
  }else if (tac->op1->type == SYMBOL_LIT_INT || tac->op1->type == SYMBOL_LIT_BYTE){
    num1 = leapNumToDecNum(tac->op1->text);
    fprintf(output, "\tmovl $%s, %%eax\n", num1);
  }

  if (svar){
    fprintf(output, "\tmovl %s(%%rip), %%esi\n", tac->op2->text);
  }else if (tac->op2->type == SYMBOL_LIT_INT || tac->op2->type == SYMBOL_LIT_BYTE){
    num2 = leapNumToDecNum(tac->op2->text);
    fprintf(output, "\tmovl $%s, %%esi\n", num2);
  }

  fprintf(output, "\tcltd\n"
                  "\tidivl %%esi\n");

  free(num1);
  free(num2);
}

void createIFZ(TAC* tac, FILE* output){
  fprintf(output, "\ttest %%eax, %%eax\n"
                  "\tje %s\n", tac->result->text);
}

void createCALL(TAC* tac, FILE* output){
  fprintf(output, "\tcall %s\n", tac->op1->text);
}

void createREAD(TAC* tac, FILE *output){
  if (!tac) return;
  if (tac->result->type != SYMBOL_VAR) return;
  if (strstr(tac->result->text, "__tempvar")) return;

  fprintf(output, STR_READ, tac->result->text);
}
