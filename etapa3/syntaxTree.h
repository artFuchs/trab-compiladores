#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include "hash.h"

typedef struct syntax_node{
  int type;
  NODE* content; // case type == LIT or IDENTIFIER
  struct syntax_node* son1;
  struct syntax_node* son2;
  struct syntax_node* son3;
  struct syntax_node* son4;
} SYNTREE;

SYNTREE* newSyntaxNode (int type, NODE* content, SYNTREE* son1, SYNTREE* son2, SYNTREE* son3, SYNTREE* son4);
void printNode (SYNTREE* node);
void printTree (SYNTREE* root);

#endif
