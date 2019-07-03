/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <stdlib.h>
#include <stdio.h>
#include "ast_const.h"
#include "hash.h"

typedef struct syntax_node{
  int type;
  int dataType;
  int lineNumber;
  struct node *symbol; // case type == LIT or IDENTIFIER
  struct syntax_node *sons [MAX_SONS];
} AST;

AST* global_ast;
AST** functions_asts;
int functions_asts_size;

AST* newSyntaxNode (int type, struct node* symbol, AST* son1, AST* son2, AST* son3, AST* son4, int lineNumber);
void printNode (AST* node, int depth);
void printTree (AST* root, int depth);
void decompile (AST* node, FILE* output);

#endif
