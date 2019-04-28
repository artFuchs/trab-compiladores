#include "ast.h"

AST* newSyntaxNode (int type, NODE* symbol, AST* son1, AST* son2, AST* son3, AST* son4){
  AST* newNode = 0;
  newNode = (AST*) calloc (1,sizeof(AST));
  newNode->type = type;
  newNode->symbol = symbol;
  newNode->son1 = son1;
  newNode->son2 = son2;
  newNode->son3 = son3;
  newNode->son4 = son4;
  return newNode;
}

void printNode (AST* node){

}

void printTree (AST* root){

}
