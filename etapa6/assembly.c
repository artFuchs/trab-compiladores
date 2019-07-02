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
char* leapNumToInt(char *num);
int createAssembly(TAC *tac, FILE *output);
void createVARDECL(TAC *tac, FILE *output);
void createBEGFUN(TAC *tac, FILE *output);
void createENDFUN(TAC *tac, FILE *output);
void createRETURN(TAC *tac, FILE *output);
void createPRINT(TAC *tac, FILE *output);
void createMOVE(TAC *tac, FILE *output);
void createBinop(int op, TAC *tac, FILE *output);


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

char* leapNumToInt(char *num){
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
      default: buffer[i] = 0;
    }
  }
  return buffer;
}

int createAssembly(TAC *tac, FILE *output){
  if (!tac) return 0;

  switch (tac->instruction){
    case TAC_BYTE_DECL:
    case TAC_INT_DECL:
    case TAC_FLOAT_DECL: createVARDECL(tac, output); break;
    case TAC_BEGINFUN: createBEGFUN(tac, output); break;
    case TAC_RETURN: createRETURN(tac, output); break;
    case TAC_ENDFUN: createENDFUN(tac, output); break;
    case TAC_PRINT: createPRINT(tac, output); break;
    case TAC_MOVE: createMOVE(tac, output); break;
    case TAC_ADD:
    case TAC_SUB: createBinop(tac->instruction, tac, output); break;
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
    char *num = leapNumToInt(tac->op1->text);
    sprintf (val, ".long %s", num);
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
        num = leapNumToInt(tac->result->text);
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
    case SYMBOL_LIT_FLOAT:
    case SYMBOL_LIT_BYTE:
      num = leapNumToInt(tac->result->text);
      fprintf(output,
              STR_PRINT_NUM,
              num);
      free(num);
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
}

void createMOVE(TAC *tac, FILE *output){
  char* num = NULL;
  switch (tac->op1->type){
    case SYMBOL_LIT_INT:
    case SYMBOL_LIT_BYTE:
      num = leapNumToInt(tac->op1->text);
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

void createBinopVar(int op, FILE *output){
  switch (op){
    case TAC_ADD:
      fprintf(output, "\taddl %%edx, %%eax\n");
      break;
    case TAC_SUB:
      fprintf(output, "\tsubl %%eax, %%edx\n"
                      "\tmovl %%edx, %%eax\n");
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
  }
}

void createBinop(int op, TAC *tac, FILE *output){
  int fvar = tac->op1->type == SYMBOL_VAR;
  int svar = tac->op2->type == SYMBOL_VAR;
  int dvars = fvar && svar;
  if (dvars){
    fprintf(output,
            "\tmovl %s(%%rip), %%edx\n"
            "\tmovl %s(%%rip), %%eax\n",
            tac->op1->text, tac->op2->text);
    createBinopVar(op, output);
  }else if (fvar){
    char *num = NULL;
    switch(tac->op2->type){
      case SYMBOL_LIT_BYTE:
      case SYMBOL_LIT_INT:
        num = leapNumToInt(tac->op2->text);
        fprintf(output, "\tmovl %s(%%rip), %%eax\n", tac->op1->text);
        createBinopNum(op, num, output);
        free(num);
    }
  }else if (svar){
    char *num = NULL;
    switch(tac->op1->type){
      case SYMBOL_LIT_BYTE:
      case SYMBOL_LIT_INT:
        num = leapNumToInt(tac->op1->text);
        fprintf(output, "\tmovl $%s, %%edx\n", num);
        fprintf(output, "\tmovl %s(%%rip), %%eax\n", tac->op2->text);
        createBinopVar(op, output);
    }
    free(num);
  }else{
      char *num1 = NULL;
      char *num2 = NULL;
      if (tac->op1->type == SYMBOL_LIT_INT || tac->op1->type == SYMBOL_LIT_BYTE)
        num1 = leapNumToInt(tac->op1->text);
      else
        return;

      if (tac->op2->type == SYMBOL_LIT_INT || tac->op2->type == SYMBOL_LIT_BYTE)
        num2 = leapNumToInt(tac->op2->text);
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
