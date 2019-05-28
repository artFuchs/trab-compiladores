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
  NODE* symbol; // case type == LIT or IDENTIFIER
  struct syntax_node* sons [MAX_SONS];
} AST;

AST* global_ast;

AST* newSyntaxNode (int type, NODE* symbol, AST* son1, AST* son2, AST* son3, AST* son4);
void printNode (AST* node, int depth);
void printTree (AST* root, int depth);
void decompile (AST* node, FILE* output);

#endif
