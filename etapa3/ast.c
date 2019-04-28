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

void printNode (AST* node, int depth){
  if (node == NULL) return;
  int i = 0;
  for (i=0;i<depth;i++){
    printf("| ");
  }
  switch (node->type){
    case 0: printf("AST_SYMBOL"); break;
    default: printf("AST_INVALID"); break;
  }
  if (node->symbol!=0){
    printf (", %s", node->symbol->text);
  }
  printf("\n");
}

void printTree (AST* root, int depth){
  if (root == NULL) return;
  printNode(root, depth);
  printTree(root->son1, depth+1);
  printTree(root->son2, depth+1);
  printTree(root->son3, depth+1);
  printTree(root->son4, depth+1);
}
