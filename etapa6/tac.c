#include "tac.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TAC* genSymbolTac(AST* ast, TAC **code);
TAC* genVarDeclTac(AST *ast, TAC **code);
TAC* genArrayDeclTac(AST *ast);
TAC* genFuncDeclTac(AST *ast, TAC **code);
TAC* genUnopTac(int inst, TAC **code);
TAC* genBinopTac(int inst, TAC **code);
TAC* genMoveTac(AST* ast, TAC **code);
TAC* genArrayWriteTac(AST *ast, TAC **code);
TAC* genFuncCallTac(AST *ast, TAC **code);
TAC* genFuncCallArgTac(AST *ast, AST *func);
TAC* genIfTac(TAC **code);
TAC* genIfElseTac(TAC **code);
TAC* genLoopTac(TAC **code);
TAC* genPrintTac(TAC **code);
TAC* genReturnTac(TAC **code);
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
      case TAC_INT_DECL:  printf("INT "); break;
      case TAC_BYTE_DECL:  printf("BYTE "); break;
      case TAC_FLOAT_DECL:  printf("FLOAT "); break;
      case TAC_BYTEA_DECL:  printf("BYTEA "); break;
      case TAC_INTA_DECL:  printf("INTA "); break;
      case TAC_FLOATA_DECL:  printf("FLOATA "); break;
      case TAC_ARRAY_INIT:  printf("ARRAYINIT "); break;
      case TAC_MOVE:      printf("MOVE "); break;
      case TAC_ARRAYW:    printf("ARRAYWRITE "); break;
      case TAC_ARRAYR:    printf("ARRAYREAD "); break;
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
      case TAC_BEGINFUN:  printf("\nBEGINFUN "); break;
      case TAC_ENDFUN:    printf("ENDFUN "); break;
      case TAC_ARGREAD:   printf("ARGREAD "); break;
      case TAC_ARGWRITE:  printf("ARGWRITE "); break;
      case TAC_IFZ:       printf("IFZ "); break;
      case TAC_JUMP:      printf("JUMP "); break;
      case TAC_LABEL:     printf("LABEL "); break;
      case TAC_READ:      printf("READ "); break;
      case TAC_PRINT:     printf("PRINT "); break;
      case TAC_RETURN:    printf("RETURN "); break;
    }

    if (tac->instruction){
        if (tac->result) printf("%s ", tac->result->text);
        if (tac->op1) printf("%s ", tac->op1->text);
        if (tac->op2) printf("%s ", tac->op2->text);
        printf("\n");
        if (tac->instruction == TAC_ENDFUN)
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
    case AST_SYMBOL: result = genSymbolTac(ast,code); break;
    case AST_PARAM_ELEM:
    case AST_VAR_DECL: result = genVarDeclTac(ast,code); break;
    case AST_ARRAY_DECL: result = genArrayDeclTac(ast); break;
    case AST_FUNC_DECL: result = genFuncDeclTac(ast,code); break;
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
    //case AST_ARG_LIST: result = genFuncCallArgTac(ast,code); break;
    case AST_IF: result = genIfTac(code); break;
    case AST_IF_ELSE: result = genIfElseTac(code); break;
    case AST_LOOP: result = genLoopTac(code); break;
    case AST_LEAP: break;
    case AST_PRINT: result = genPrintTac(code); break;
    case AST_READ: result = tacCreate(TAC_READ,ast->symbol,0,0); break;
    case AST_RETURN: result = genReturnTac(code); break;
    default:
      result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
  }

  return result;
}

TAC* lastTAC(TAC* root){
  if (!root) return 0;
  TAC* aux= root;
  while (aux->next) aux = aux->next;
  return aux;
}

TAC* genSymbolTac(AST *ast, TAC **code){
  TAC* symbol = 0;
  if (ast->symbol->type == SYMBOL_ARRAY){
    symbol = tacCreate(TAC_ARRAYR, tempCreate(),
                                     ast->symbol,
                                     code[0]?code[0]->result:0);
  }else{
    symbol = tacCreate(TAC_SYMBOL, ast->symbol, 0, 0);
  }
  return symbol;
}

TAC* genVarDeclTac(AST *ast, TAC **code){
  int type;
  switch (ast->sons[0]->type){
    case AST_TBYTE: type = TAC_BYTE_DECL; break;
    case AST_TINT: type = TAC_INT_DECL; break;
    case AST_TFLOAT: type = TAC_FLOAT_DECL; break;
  }
  return tacCreate(type,ast->symbol,code[1]?code[1]->result:0,0);
}

TAC* genArrayDeclTac(AST *ast){
  int type;
  switch (ast->sons[0]->type){
    case AST_TBYTE: type = TAC_BYTEA_DECL; break;
    case AST_TINT: type = TAC_INTA_DECL; break;
    case AST_TFLOAT: type = TAC_FLOATA_DECL; break;
  }
  TAC* decl = tacCreate(type,ast->symbol,ast->sons[1]?ast->sons[1]->symbol:0,0);
  AST* aux = ast->sons[2];
  TAC* init = NULL;
  while (aux){
    TAC* initPos = tacCreate(TAC_ARRAY_INIT,ast->symbol,aux->sons[0]?aux->sons[0]->symbol:0,0);
    init = tacJoin(init,initPos);
    aux = aux->sons[1];
  }
  return tacJoin(decl,init);
}

TAC* genUnopTac(int inst, TAC **code){
  TAC* aux = lastTAC(code[0]);
  TAC* newTac = tacCreate(inst, tempCreate(), aux ? aux->result : 0, 0);
  return tacJoin(aux, newTac);
}

TAC* genBinopTac(int inst, TAC **code){
  TAC* aux0 = lastTAC(code[0]);
  TAC* aux1 = lastTAC(code[1]);
  if (aux1) {
    while (aux1->next) aux1 = aux1->next;
  }
  TAC* newTac = tacCreate(inst, tempCreate(), aux0 ? aux0->result : 0, aux1 ? aux1->result : 0);
  return tacJoin(code[0], tacJoin(code[1],newTac));
}

TAC* genMoveTac(AST* ast, TAC **code){
  TAC* aux = lastTAC(code[0]);
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
  char label[80];
  sprintf(label,"%s",ast->symbol->text);
  NODE* labelNode = labelCreate(label);

  TAC* args = 0;
  if (ast->sons[0]){
    // find the right function AST
    AST* func = 0;
    int i;
    for (i=0; i<functions_asts_size; i++){
      if (strcmp(functions_asts[i]->symbol->text,ast->symbol->text)==0){
        func = functions_asts[i];
        break;
      }
    }
    args = genFuncCallArgTac(ast->sons[0], func);
  }

  TAC* fun = tacCreate(TAC_CALL,tempCreate(),labelNode, 0);
  AST* aux = ast->sons[0];
  return tacJoin(code[0],
                  tacJoin(args, fun));
}

TAC* genFuncCallArgTac(AST *ast, AST *func){
  if (!ast || !func) return 0;
  TAC* args = NULL;
  AST* aux = ast;
  AST* auxF = func->sons[1];
  while (aux){
    args = tacJoin(tacCreate(TAC_ARGWRITE, auxF->sons[0]?auxF->sons[0]->symbol:0, aux->sons[0]->symbol?aux->sons[0]->symbol:0, 0), args);
    aux = aux->sons[1];
    auxF = auxF->sons[1];
  }
  return args;
}

TAC* genFuncDeclTac(AST *ast, TAC **code){
  char label[80];
  sprintf(label,"%s",ast->symbol->text);
  NODE* labelNode = labelCreate(label);
  TAC* funcBegin = tacCreate(TAC_BEGINFUN, labelNode, 0, 0);
  TAC* funcEnd = tacCreate(TAC_ENDFUN, labelNode, 0, 0);
  return tacJoin(code[1],
          tacJoin(funcBegin,
            tacJoin(code[2], funcEnd)));
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

TAC* genPrintTac(TAC **code){
  TAC* aux = code[0];
  TAC* printTacs = NULL;
  while (aux){
    TAC* aux1 = tacCreate(TAC_PRINT, aux->result,0,0);
    printTacs = tacJoin(printTacs, aux1);
    aux = aux->next;
  }
  return tacJoin(code[0],printTacs);
}

TAC* genReturnTac(TAC **code){
  TAC* aux = lastTAC(code[0]);
  TAC* returnTac = tacCreate(TAC_RETURN, aux?aux->result:0, 0, 0);
  return tacJoin(code[0], returnTac);
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
    sprintf(text,".LABEL%d",labelnum);
    labelnum++;
  }
  return hashInsert(SYMBOL_VAR,0,text);
}
