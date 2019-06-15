/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#ifndef TAC_H
#define TAC_H

#define TAC_SYMBOL    0

#define TAC_MOVE      1
#define TAC_ARRAYW    2
#define TAC_ARRAYR    3

#define TAC_ADD       11
#define TAC_SUB       12
#define TAC_MUL       13
#define TAC_DIV       14

#define TAC_AND       20
#define TAC_OR        21
#define TAC_NOT       22
#define TAC_LT        23
#define TAC_GT        24
#define TAC_LE        25
#define TAC_GE        26
#define TAC_EQ        27
#define TAC_NEQ       28

#define TAC_CALL      30
#define TAC_BEGINFUN  31
#define TAC_ENDFUN    32
#define TAC_ARG       33

#define TAC_IFZ       40
#define TAC_JUMP      41
#define TAC_LABEL     42

#define TAC_READ      50
#define TAC_PRINT     51

#include "hash.h"

typedef struct tac_node {
  int instruction;
  NODE *result;
  NODE *op1;
  NODE *op2;
  struct tac_node *next;
} TAC;

// tacCreate
// given the necessary information, alloc and return a pointer to a TAC node
TAC* tacCreate(int instruction, NODE *result, NODE *op1, NODE *op2);

// tacJoin
// given two TACs lists, concatenate the two lists, assigning snd as the next of
// the lTAC element of the first list;
TAC* tacJoin(TAC *fst, TAC *snd);

// tacPrint
// navigate the TAC list and print the elements
void tacPrint(TAC *head);

// genTacs
// navigate an AST, generating a TACs list;
TAC* genTac(AST *ast);

#endif
