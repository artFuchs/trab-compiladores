/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#include "ast.h"
#include <stdio.h>

AST* newSyntaxNode (int type, NODE* symbol, AST* son1, AST* son2, AST* son3, AST* son4){
  AST* newNode = 0;
  newNode = (AST*) calloc (1,sizeof(AST));
  newNode->type = type;
  newNode->symbol = symbol;
  newNode->sons[0] = son1;
  newNode->sons[1] = son2;
  newNode->sons[2] = son3;
  newNode->sons[3] = son4;
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
    case AST_VAR_DECL: printf("AST_VAR_DECL"); break;
    case AST_ARRAY_DECL: printf("AST_ARRAY_DECL"); break;
    case AST_FUNC_DECL: printf("AST_FUNC_DECL"); break;
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
    case AST_NEQ: printf("AST_NEQ"); break;
    case AST_ASSIGN: printf("AST_ASSIGN"); break;
    case AST_ARRAY_ASSIGN: printf("AST_ARRAY_ASSIGN"); break;
    case AST_FUNC_CALL: printf("AST_FUNC_CALL"); break;
    case AST_IF: printf("AST_IF"); break;
    case AST_IF_ELSE: printf("AST_IF_ELSE"); break;
    case AST_LOOP: printf("AST_LOOP"); break;
    case AST_LEAP: printf("AST_LEAP"); break;
    case AST_NO_DELIMITER_LIST: printf("AST_NO_DELIMITER_LIST"); break;
    case AST_COMMA_DELIMITED_LIST: printf("AST_COMMA_DELIMITED_LIST"); break;
    case AST_SEMICOLON_DELIMITED_LIST: printf("AST_SEMICOLON_DELIMITED_LIST"); break;
    case AST_PARAM_ELEM: printf("AST_PARAM_ELEM"); break;
    case AST_PRINT: printf("AST_PRINT"); break;
    case AST_READ: printf("AST_READ"); break;
    case AST_RETURN: printf("AST_RETURN"); break;
    case AST_CMD_BLOCK: printf("AST_CMD_BLOCK"); break;
    case AST_PARENTHESES: printf("AST_PARENTHESES"); break;
    case AST_INTEGER: printf("AST_INTEGER"); break;
    case AST_FLOAT: printf("AST_FLOAT"); break;
    case AST_CHAR: printf("AST_CHAR"); break;
    case AST_STRING: printf("AST_STRING"); break;
    case AST_TINT: printf("AST_TINT"); break;
    case AST_TFLOAT: printf("AST_TFLOAT"); break;
    case AST_TBYTE: printf("AST_TBYTE"); break;
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
  int i;
  for (i=0; i<4; i++)
    printTree(root->sons[i], depth+1);
}

void decompile (AST* node, FILE* output){
  if (node == NULL || output == NULL) return;
  switch (node->type){
    case AST_SYMBOL:
      fprintf (output, "%s", node->symbol->text);
      if (node->sons[0]){
        fprintf (output, "[ ");
        decompile (node->sons[0], output);
        fprintf (output, " ]");
      }
      break;
    case AST_VAR_DECL:
      decompile (node->sons[0], output);
      fprintf (output, " %s = ", node->symbol->text);
      decompile (node->sons[1], output);
      break;
    case AST_ARRAY_DECL:
      decompile (node->sons[0], output);
      fprintf (output, " %s [ ", node->symbol->text);
      decompile (node->sons[1], output);
      fprintf (output, " ]");
      if (node->sons[2]){
        fprintf(output, ": ");
        decompile(node->sons[2], output);
      }
      break;
    case AST_FUNC_DECL:
      decompile (node->sons[0], output);
      fprintf (output, " %s ( ", node->symbol->text);
      decompile (node->sons[1], output);
      fprintf (output, ")");
      decompile (node->sons[2], output);
      break;
    case AST_ADD:
      decompile (node->sons[0], output);
      fprintf (output, " + ");
      decompile (node->sons[1], output);
      break;
    case AST_SUB:
      decompile (node->sons[0], output);
      fprintf (output, " - ");
      decompile (node->sons[1], output);
      break;
    case AST_MUL:
      decompile (node->sons[0], output);
      fprintf (output, " * ");
      decompile (node->sons[1], output);
      break;
    case AST_DIV:
      decompile (node->sons[0], output);
      fprintf (output, " / ");
      decompile (node->sons[1], output);
      break;
    case AST_AND:
      decompile (node->sons[0], output);
      fprintf (output, " and ");
      decompile (node->sons[1], output);
      break;
    case AST_OR:
      decompile (node->sons[0], output);
      fprintf (output, " or ");
      decompile (node->sons[1], output);
      break;
    case AST_NOT:
      fprintf (output, " not ");
      decompile (node->sons[0], output);
      break;
    case AST_LT:
      decompile (node->sons[0], output);
      fprintf (output, " < ");
      decompile (node->sons[1], output);
      break;
    case AST_GT:
      decompile (node->sons[0], output);
      fprintf (output, " > ");
      decompile (node->sons[1], output);
      break;
    case AST_LE:
      decompile (node->sons[0], output);
      fprintf (output, " <= ");
      decompile (node->sons[1], output);
      break;
    case AST_GE:
      decompile (node->sons[0], output);
      fprintf (output, " >= ");
      decompile (node->sons[1], output);
      break;
    case AST_EQ:
      decompile (node->sons[0], output);
      fprintf (output, " == ");
      decompile (node->sons[1], output);
      break;
    case AST_NEQ:
      decompile (node->sons[0], output);
      fprintf (output, " != ");
      decompile (node->sons[1], output);
      break;
    case AST_ASSIGN:
      fprintf (output, "%s = ", node->symbol->text);
      decompile (node->sons[0], output);
      break;
    case AST_ARRAY_ASSIGN:
      fprintf (output, "%s [ ", node->symbol->text);
      decompile (node->sons[0], output);
      fprintf (output, " ] = ");
      decompile (node->sons[1], output);
      break;
    case AST_FUNC_CALL:
      fprintf (output, "%s (", node->symbol->text);
      decompile (node->sons[0], output);
      fprintf (output, ")");
      break;
    case AST_IF:
      fprintf (output, "if ( ");
      decompile (node->sons[0], output);
      fprintf (output, ") then ");
      decompile (node->sons[1], output);
      break;
    case AST_IF_ELSE:
      fprintf (output, "if ( ");
      decompile (node->sons[0], output);
      fprintf (output, ") then ");
      decompile (node->sons[1], output);
      fprintf (output, " else ");
      decompile (node->sons[2], output);
      break;
    case AST_LOOP:
      fprintf (output, "loop ( ");
      decompile (node->sons[0], output);
      fprintf (output, ") ");
      decompile (node->sons[1], output);
      break;
    case AST_LEAP:
      fprintf (output, "leap");
      break;
    case AST_NO_DELIMITER_LIST:
      decompile (node->sons[0], output);
      fprintf (output, " ");
      decompile (node->sons[1], output);
      break;
    case AST_COMMA_DELIMITED_LIST:
      fprintf (output, ", ");
      decompile (node->sons[0], output);
      decompile (node->sons[1], output);
      break;
    case AST_SEMICOLON_DELIMITED_LIST:
      decompile (node->sons[0], output);
      fprintf (output, "; \n");
      decompile (node->sons[1], output);
      break;
    case AST_PARAM_ELEM:
      decompile (node->sons[0], output);
      fprintf (output, " %s", node->symbol->text);
      break;
    case AST_PRINT:
      fprintf (output, "print ");
      decompile (node->sons[0], output);
      break;
    case AST_READ:
      fprintf (output, "read %s ", node->symbol->text);
      break;
    case AST_RETURN:
      fprintf (output, "return ");
      decompile (node->sons[0], output);
      break;
    case AST_CMD_BLOCK:
      fprintf (output, "{");
      decompile (node->sons[0], output);
      fprintf (output, "}");
      break;
    case AST_PARENTHESES:
      fprintf (output, "(");
      decompile (node->sons[0], output);
      fprintf (output, ")");
      break;
    case AST_INTEGER:
    case AST_FLOAT:
    case AST_CHAR:
      fprintf (output, "%s", node->symbol->text);
      break;
    case AST_STRING:
      fprintf (output, "\"%s\"", node->symbol->text);
      break;
    case AST_TINT:
      fprintf (output, "int");
      break;
    case AST_TFLOAT:
      fprintf (output, "float");
      break;
    case AST_TBYTE:
      fprintf (output, "byte");
      break;
    default: return;
  }
}
