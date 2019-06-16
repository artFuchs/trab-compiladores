/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#ifndef TAC_H
#define TAC_H

#define TAC_SYMBOL     0
#define TAC_BYTE_DECL  1
#define TAC_INT_DECL   2
#define TAC_FLOAT_DECL 3
#define TAC_BYTEA_DECL 4
#define TAC_INTA_DECL 5
#define TAC_FLOATA_DECL 6
#define TAC_ARRAY_INIT 7

#define TAC_MOVE      11
#define TAC_ARRAYW    12
#define TAC_ARRAYR    13

#define TAC_ADD       21
#define TAC_SUB       22
#define TAC_MUL       23
#define TAC_DIV       24

#define TAC_AND       30
#define TAC_OR        31
#define TAC_NOT       32
#define TAC_LT        33
#define TAC_GT        34
#define TAC_LE        35
#define TAC_GE        36
#define TAC_EQ        37
#define TAC_NEQ       38

#define TAC_CALL      40
#define TAC_BEGINFUN  41
#define TAC_ENDFUN    42
#define TAC_ARG       43

#define TAC_IFZ       50
#define TAC_JUMP      51
#define TAC_LABEL     52

#define TAC_READ      60
#define TAC_PRINT     61
#define TAC_RETURN    62

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
