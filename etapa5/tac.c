#include "tac.h"
#include <stdio.h>
#include <stdlib.h>

TAC* tacCreate(int instruction, NODE *result, NODE *op1, NODE *op2){
  TAC *tac = (TAC*) calloc (1, sizeof(TAC));
  tac->instruction = instruction;
  tac->result = result;
  tac->op1 = op1;
  tac->op2 = op2;
  tac->next = NULL;
  return tac;
}

TAC* tacJoin(TAC *fst, TAC *snd){
  if (fst->next){
    TAC *aux = fst->next;
    while (aux->next){
      aux = aux->next;
    }
    aux->next = snd;
  }else{
    fst->next = snd;
  }
  return fst;
}

void tacPrint(TAC *head){
  TAC* tac = head;
  while (tac){
    switch (tac->instruction){
      case TAC_SYMBOL:
        printf("SYMBOL %s\n", tac->op1->text); break;
      case TAC_MOVE:
        printf("MOVE %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_ADD:
        printf("ADD %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_SUB:
        printf("SUB %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_MUL:
        printf("MUL %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_DIV:
        printf("DIV %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_AND:
        printf("AND %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_OR:
        printf("OR %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_NOT:
        printf("NOT %s\n", tac->op1->text); break;
      case TAC_LT:
        printf("LT %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_GT:
        printf("GT %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_LE:
        printf("LE %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_GE:
        printf("GE %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_EQ:
        printf("EQ %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_NEQ:
        printf("NEQ %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_CALL:
        printf("CALL %s \n", tac->op1->text); break;
      case TAC_BEGINFUN:
        printf("BEGINFUN %s \n", tac->op1->text); break;
      case TAC_ENDFUN:
        printf("ENDFUN %s \n", tac->op1->text); break;
      case TAC_ARG:
        printf("ARG %s \n", tac->op1->text); break;
      case TAC_IFZ:
        printf("IFZ %s %s\n", tac->op1->text, tac->op2->text); break;
      case TAC_JUMP:
        printf("JUMP %s\n", tac->op1->text); break;
      case TAC_LABEL:
        printf("LABEL %s\n", tac->op1->text); break;
      case TAC_READ:
        printf("READ %s\n", tac->op1->text); break;
      case TAC_PRINT:
        printf("PRINT %s\n", tac->op1->text); break;
    }
    tac=tac->next;
  }
}
