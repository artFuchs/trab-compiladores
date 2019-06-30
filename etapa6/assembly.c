#include <stdio.h>
#include <string.h>
#include "assembly.h"
#include "hash.h"
#include "symbols.h"

void declareStrings();
int createAssembly(TAC *tac);
void createBEGFUN(TAC *tac);
void createENDFUN(TAC *tac);
void createRETURN(TAC *tac);
void createPRINT(TAC *tac);


int tacToAssembly(TAC* tac){
  if (!tac) return 1;

  declareStrings();

  if (createAssembly(tac)) return 2;

  return 0;
}


void declareStrings(){
  int i=0;
  static int lbl_num = 0;
  fprintf(stderr,
          "\t.text\n"
          "LCNUM:\n"
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


int createAssembly(TAC *tac){
  if (!tac) return 0;

  switch (tac->instruction){
    case TAC_BEGINFUN: createBEGFUN(tac); break;
    case TAC_RETURN: createRETURN(tac); break;
    case TAC_ENDFUN: createENDFUN(tac); break;
    case TAC_PRINT: createPRINT(tac); break;
    default:  break;
  }

  createAssembly(tac->next);
  return 0;
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
  printf( "\tpopq	%%rbp\n"
          "\tret \n"
          "\t.size %s, .-%s\n",
          tac->result->text, tac->result->text);
}

void createRETURN(TAC *tac){
  if (!strstr(tac->result->text,"__tempvar")){
    printf("movl	%s(%%rip), %%eax \n",
           tac->result->text);
}

void createPRINT(TAC *tac){
  if (tac->result->type == SYMBOL_LIT_STRING){
    fprintf(stderr,
            "\tleaq %s(%%rip), %%rdi\n"
            "\tcall puts@PLT\n",
            tac->result->text);
  }
  else{
    //fazer caso para printar variaveis
  }
}
