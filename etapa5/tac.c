#include "tac.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

TAC* genUnopTac(int inst, TAC **code);
TAC* genBinopTac(int inst, TAC **code);
TAC* genMoveTac(AST* ast, TAC **code);
TAC* genArrayWriteTac(AST *ast, TAC **code);
TAC* genFuncCallTac(AST *ast, TAC **code);
TAC* genIfTac(TAC **code);
TAC* genIfElseTac(TAC **code);
TAC* genLoopTac(TAC **code);
NODE* tempCreate();
NODE* labelCreate(char *labelText);

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
  if (!fst) return snd;
  if (!snd) return fst;
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
  fprintf(stderr, "\n\tTAC CODE:\n");
  while (tac){
    switch (tac->instruction){
      case TAC_MOVE:      printf("MOVE "); break;
      case TAC_ARRAYW:    printf("ARRAYWRITE "); break;
      case TAC_ADD:       printf("ADD "); break;
      case TAC_SUB:       printf("SUB "); break;
      case TAC_MUL:       printf("MUL "); break;
      case TAC_DIV:       printf("DIV "); break;
      case TAC_AND:       printf("AND "); break;
      case TAC_OR:        printf("OR "); break;
      case TAC_NOT:       printf("NOT "); break;
      case TAC_LT:        printf("LT "); break;
      case TAC_GT:        printf("GT "); break;
      case TAC_LE:        printf("LE "); break;
      case TAC_GE:        printf("GE "); break;
      case TAC_EQ:        printf("EQ "); break;
      case TAC_NEQ:       printf("NEQ "); break;
      case TAC_CALL:      printf("CALL "); break;
      case TAC_BEGINFUN:  printf("BEGINFUN "); break;
      case TAC_ENDFUN:    printf("ENDFUN "); break;
      case TAC_ARG:       printf("ARG "); break;
      case TAC_IFZ:       printf("IFZ "); break;
      case TAC_JUMP:      printf("JUMP "); break;
      case TAC_LABEL:     printf("LABEL "); break;
      case TAC_READ:      printf("READ "); break;
      case TAC_PRINT:     printf("PRINT "); break;
    }

    switch (tac->instruction){
      case TAC_SYMBOL:
      case TAC_ARRAYR:
        break;
      default:
        if (tac->result) printf("%s ", tac->result->text);
        if (tac->op1) printf("%s ", tac->op1->text);
        if (tac->op2) printf("%s ", tac->op2->text);
        printf("\n");
    }

    tac=tac->next;
  }
}

TAC* genTac(AST *ast){
  TAC *code[MAX_SONS];
  TAC *result = NULL;
  int i;

  if (!ast) return NULL;
  for (i=0;i<MAX_SONS;i++){
    code[i]=genTac(ast->sons[i]);
  }

  switch (ast->type) {
    case AST_SYMBOL:
      if (ast->symbol->type == SYMBOL_ARRAY){
        TAC* vec = tacCreate(TAC_ARRAYR, tempCreate(),
                                         ast->symbol,
                                         code[0]?code[0]->result:0);
        result = tacJoin(code[0], vec);
      }else{
        result = tacCreate(TAC_SYMBOL, ast->symbol, 0, 0); break;
      }
      break;
    case AST_PARAM_ELEM:
    case AST_INTEGER:
    case AST_FLOAT:
    case AST_CHAR:
    case AST_STRING:
      result = tacCreate(TAC_SYMBOL, ast->symbol, 0, 0); break;
    case AST_ADD: result = genBinopTac(TAC_ADD, code); break;
    case AST_SUB: result = genBinopTac(TAC_SUB, code); break;
    case AST_MUL: result = genBinopTac(TAC_MUL, code); break;
    case AST_DIV: result = genBinopTac(TAC_DIV, code); break;
    case AST_AND: result = genBinopTac(TAC_AND, code); break;
    case AST_OR: result = genBinopTac(TAC_OR, code); break;
    case AST_NOT: result = genUnopTac(TAC_NOT, code); break;
    case AST_LT: result = genBinopTac(TAC_LT, code); break;
    case AST_GT: result = genBinopTac(TAC_GT, code); break;
    case AST_LE: result = genBinopTac(TAC_LE, code); break;
    case AST_GE: result = genBinopTac(TAC_GE, code); break;
    case AST_EQ: result = genBinopTac(TAC_EQ, code); break;
    case AST_NEQ: result = genBinopTac(TAC_NEQ, code); break;
    case AST_ASSIGN: result = genMoveTac(ast, code); break;
    case AST_ARRAY_ASSIGN: result = genArrayWriteTac(ast, code); break;
    case AST_FUNC_CALL: result = genFuncCallTac(ast,code); break;
    case AST_IF: result = genIfTac(code); break;
    case AST_IF_ELSE: result = genIfElseTac(code); break;
    case AST_LOOP: result = genLoopTac(code); break;
    case AST_LEAP: break;
    default:
      result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
  }

  return result;
}

TAC* lastTAC(TAC* root){
  TAC* aux= root;
  while (aux->next) aux = aux->next;
  return aux;
}

TAC* genUnopTac(int inst, TAC **code){
  TAC* aux = lastTAC(code[0]);
  TAC* newTac = tacCreate(inst, tempCreate(), aux ? aux->result : 0, 0);
  return tacJoin(aux, newTac);
}

TAC* genBinopTac(int inst, TAC **code){
  TAC* aux0 = lastTAC(code[0]);
  TAC* aux1 = lastTAC(code[1]);
  while (aux1->next) aux1 = aux1->next;
  TAC* newTac = tacCreate(inst, tempCreate(), aux0 ? aux0->result : 0, aux1 ? aux1->result : 0);
  return tacJoin(code[0], tacJoin(code[1],newTac));
}

TAC* genMoveTac(AST* ast, TAC **code){
  TAC* aux= lastTAC(code[0]);
  TAC* mv = tacCreate(TAC_MOVE, ast->symbol,
                                aux ? aux->result : 0, 0);
  return tacJoin(code[0], mv);
}

TAC* genArrayWriteTac(AST *ast, TAC **code){
  TAC* aux0 = lastTAC(code[0]);
  TAC* aux1 = lastTAC(code[1]);
  TAC* mv = tacCreate(TAC_ARRAYW, ast->symbol,
                                  aux0 ? aux0->result : 0,
                                  aux1 ? aux1->result : 0);
  return tacJoin(code[0], tacJoin(code[1], mv));
}

TAC* genFuncCallTac(AST *ast, TAC **code){
  TAC* fun = tacCreate(TAC_CALL,tempCreate(),ast->symbol, 0);
  return tacJoin(code[0],fun);
}

TAC* genIfTac(TAC **code){
  NODE* label = labelCreate(NULL);
  TAC* aux = lastTAC(code[0]);
  TAC* ifztac = tacCreate(TAC_IFZ, label, aux?aux->result:0, 0);
  TAC* labtac = tacCreate(TAC_LABEL, label, 0, 0);
  return tacJoin(code[0],tacJoin(ifztac,tacJoin(code[1],labtac)));
}

TAC* genIfElseTac(TAC **code){
  NODE* labelelse = labelCreate(NULL);
  NODE* labelendif = labelCreate(NULL);
  TAC* aux= lastTAC(code[0]);
  TAC* ifztac = tacCreate(TAC_IFZ, labelelse, aux?aux->result:0, 0);
  TAC* jump = tacCreate(TAC_JUMP, labelendif, 0, 0);
  TAC* labElseTac = tacCreate(TAC_LABEL, labelelse, 0, 0);
  TAC* labEndifTac = tacCreate(TAC_LABEL, labelendif, 0, 0);
  return tacJoin(code[0],
                tacJoin(ifztac,
                        tacJoin(code[1],
                               tacJoin(jump,
                                       tacJoin(labElseTac,
                                             tacJoin(code[2],
                                                    labEndifTac))))));
}

TAC* genLoopTac(TAC **code){
  NODE* labelInit = labelCreate(NULL);
  NODE* labelEnd = labelCreate(NULL);
  TAC* labelInitTac = tacCreate(TAC_LABEL,labelInit,0,0);
  TAC* aux= lastTAC(code[0]);
  TAC* ifz = tacCreate(TAC_IFZ,labelEnd,aux?aux->result:0,0);
  TAC* jump = tacCreate(TAC_JUMP,labelInit,0,0);
  TAC* labelEndTac = tacCreate(TAC_LABEL,labelEnd,0,0);
  return tacJoin(labelInitTac,
                tacJoin(code[0],
                      tacJoin(ifz,
                              tacJoin(code[1],
                                      tacJoin(jump, labelEndTac)))));
}

NODE *tempCreate(){
  static int varnum = 0;
  char text[31];
  sprintf(text,"__tempvar%d",varnum);
  varnum++;
  return hashInsert(SYMBOL_VAR,0,text);
}

NODE *labelCreate(char *labelText){
  static int labelnum = 0;
  char *text;
  if (labelText)
    text = labelText;
  else{
    text = (char*) calloc (29,sizeof(char));
    sprintf(text,"#label%d",labelnum);
    labelnum++;
  }
  return hashInsert(SYMBOL_VAR,0,text);
}
