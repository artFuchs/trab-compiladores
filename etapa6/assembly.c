#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembly.h"
#include "hash.h"
#include "symbols.h"

void declareStrings();
void fixIntVarNames();
char* leapNumToInt(char *num);
int createAssembly(TAC *tac);
void createVARDECL(TAC *tac);
void createBEGFUN(TAC *tac);
void createENDFUN(TAC *tac);
void createRETURN(TAC *tac);
void createPRINT(TAC *tac);


int tacToAssembly(TAC* tac){
  if (!tac) return 1;

  declareStrings();
  fixIntVarNames();

  if (createAssembly(tac)) return 2;

  return 0;
}


void declareStrings(){
  int i=0;
  static int lbl_num = 0;
  fprintf(stderr,
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
          fprintf(stderr,
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

int createAssembly(TAC *tac){
  if (!tac) return 0;

  switch (tac->instruction){
    case TAC_BYTE_DECL:
    case TAC_INT_DECL:
    case TAC_FLOAT_DECL: createVARDECL(tac); break;
    case TAC_BEGINFUN: createBEGFUN(tac); break;
    case TAC_RETURN: createRETURN(tac); break;
    case TAC_ENDFUN: createENDFUN(tac); break;
    case TAC_PRINT: createPRINT(tac); break;
    default:  break;
  }

  createAssembly(tac->next);
  return 0;
}

void createVARDECL(TAC *tac){
  int size=4;
  char* varName;
  char zeros[8];
  char val[28] = "0";
  varName = tac->result->text;
  switch(tac->instruction){
    case TAC_BYTE_DECL: size = 1; break;
    case TAC_INT_DECL:
    case TAC_FLOAT_DECL: size = 4; break;
    default: return;
  }
  sprintf (zeros,".zero %d", size);
  if (tac->op1){
    sprintf (val, ".long %s", leapNumToInt(tac->op1->text));
  }
  fprintf(stderr,
          "\t.globl %s\n"
          "\t.align 4\n"
          "\t.size %s, %d\n"
          "%s:\n"
          "\t%s\n"
          ,varName, varName, size, varName,
          tac->op1?val:zeros);
}

void createBEGFUN(TAC *tac){
  printf( "\t.text \n"
          "\t.globl	%s\n"
          "%s:\n"
          "\tpushq	%%rbp\n"
          "\tmovq	%%rsp, %%rbp \n",
          tac->result->text,
          tac->result->text);
}

void createENDFUN(TAC *tac){
  fprintf(stderr,
          "\tpopq	%%rbp\n"
          "\tret \n"
          "\t.size %s, .-%s\n",
          tac->result->text, tac->result->text);
}

void createRETURN(TAC *tac){
  if (!strstr(tac->result->text,"__tempvar")){
    printf("movl	%s(%%rip), %%eax \n",
           tac->result->text);
  }
}

void createPRINT(TAC *tac){
  switch (tac->result->type){
    case SYMBOL_LIT_STRING:
      fprintf(stderr,
              "\tleaq %s(%%rip), %%rdi\n"
              "\tcall puts@PLT\n",
              tac->result->text);
      break;
    case SYMBOL_LIT_INT:
    case SYMBOL_LIT_FLOAT:
    case SYMBOL_LIT_BYTE:
      break;
    case SYMBOL_VAR:
      if (strstr(tac->result->text,"__tempvar")){
        //caso para tempvars
        //deve pegar valores
      }
      else{
        fprintf(stderr,
                "\tmovl %s(%%rip), %%eax\n"
                "\tmovl %%eax, %%esi\n"
                "\tleaq LCINT(%%rip), %%rdi\n"
                "\tmovl $0, %%eax\n"
                "\tcall printf@PLT\n",
                tac->result->text);
      }
      break;
  }
}
