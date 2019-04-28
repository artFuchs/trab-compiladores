#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

typedef struct syntax_node{
  int type;
  NODE* symbol; // case type == LIT or IDENTIFIER
  struct syntax_node* son1;
  struct syntax_node* son2;
  struct syntax_node* son3;
  struct syntax_node* son4;
} AST;

AST* newSyntaxNode (int type, NODE* symbol, AST* son1, AST* son2, AST* son3, AST* son4);
void printNode (AST* node, int depth);
void printTree (AST* root, int depth);

#endif
