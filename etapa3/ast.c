/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

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
    case AST_SYMBOL: printf("AST_SYMBOL"); break;
    case AST_ADD: printf("AST_ADD"); break;
    case AST_SUB: printf("AST_SUB"); break;
    case AST_MUL: printf("AST_MUL"); break;
    case AST_DIV: printf("AST_DIV"); break;
    case AST_AND: printf("AST_AND"); break;
    case AST_OR: printf("AST_OR"); break;
    case AST_NOT: printf("AST_NOT"); break;
    case AST_LT: printf("AST_LT"); break;
    case AST_GT: printf("AST_GT"); break;
    case AST_LE: printf("AST_LE"); break;
    case AST_GE: printf("AST_GE"); break;
    case AST_EQ: printf("AST_EQ"); break;
    case AST_ASSIGN: printf("AST_ASSIGN"); break;
    case AST_ARRAY_ASSIGN: printf("AST_ARRAY_ASSIGN"); break;
    case AST_IF: printf("AST_IF"); break;
    case AST_IF_ELSE: printf("AST_IF_ELSE"); break;
    case AST_LOOP: printf("AST_LOOP"); break;
    case AST_LEAP: printf("AST_LEAP"); break;
    case AST_NO_DELIMITER_LIST: printf("AST_NO_DELIMITER_LIST"); break;
    case AST_COMMA_DELIMITED_LIST: printf("AST_COMMA_DELIMITED_LIST"); break;
    case AST_SEMICOLON_DELIMITED_LIST: printf("AST_SEMICOLON_DELIMITED_LIST"); break;
    case AST_PRINT: printf("AST_PRINT"); break;
    case AST_READ: printf("AST_READ"); break;
    case AST_RETURN: printf("AST_RETURN"); break;
    case AST_CMD_BLOCK: printf("AST_CMD_BLOCK"); break;
    case AST_PARENTHESES: printf("AST_PARENTHESES"); break;
    case AST_INTEGER: printf("AST_INTEGER"); break;
    case AST_FLOAT: printf("AST_FLOAT"); break;
    case AST_CHAR: printf("AST_CHAR"); break;
    case AST_STRING: printf("AST_STRING"); break;
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
