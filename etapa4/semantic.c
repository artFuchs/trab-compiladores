#include "semantic.h"
#include "hash.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

void printError(char *msg, char *type, char *varName);

void declareSymbol(AST *node, int type){
  if (node->symbol->type == SYMBOL_FUNC){
    if (node->sons[0]->symbol->type != SYMBOL_IDENTIFIER){
      fprintf (stderr,"semantic error: param %s redeclared\n", node->sons[0]->symbol->text);
      //TODO: informar erro semantico
      return;
    }
  }
  else if (node->symbol->type != SYMBOL_IDENTIFIER){
    fprintf (stderr,"semantic error: variable %s redeclared\n", node->symbol->text);
    //TODO: informar erro semantico
    return;
  }

  switch (type){
    case AST_PARAM_ELEM:
      node->symbol->type = SYMBOL_VAR;
      // TODO: setar tipo do parametro
      break;
    case AST_VAR_DECL:
      node->symbol->type = SYMBOL_VAR;
      // TODO: setar tipo da variavel
      break;
    case AST_ARRAY_DECL:
      node->symbol->type = SYMBOL_ARRAY;
      // TODO: setar tipo do array
      break;
    case AST_FUNC_DECL:
      node->symbol->type = SYMBOL_FUNC;
      // TODO: setar tipo de retorno da função
      break;
  }
}

void hashCheckUndeclared(){
  int i;
  for (i=0; i<HASH_SIZE; i++){
    NODE *node;
    for (node = Table[i]; node; node=node->next){
      if (node->type == SYMBOL_IDENTIFIER){
        fprintf (stderr, "semantic error: var %s not declared\n", node->text);
      }
    }
  }
  return;
}

void checkSymbolsUsage(AST *node){
  if (node == 0) return;
  switch (node->type) {
    case AST_SYMBOL:
      if (node->symbol->type == SYMBOL_VAR){
        if (node->sons[0]){
          fprintf(stderr, "semantic error: var %s being indexed\n", node->symbol->text);
        }
      }
      else if (node->symbol->type == SYMBOL_ARRAY){
        if (node->sons[0]==0)
          fprintf(stderr, "semantic error: array %s not being indexed\n", node->symbol->text );
        else
          checkSymbolsUsage(node->sons[0]);
      }
      else if (node->symbol->type == SYMBOL_FUNC){
        fprintf(stderr, "semantic error: trying to use function %s as a variable\n", node->symbol->text);
      }
      break;
    case AST_FUNC_CALL:
      if (node->symbol->type != SYMBOL_FUNC)
        fprintf(stderr, "semantic error: variable %s being used as function\n", node->symbol->text);
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    case AST_AND:
    case AST_OR:
    case AST_LT:
    case AST_GT:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_NOT:
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_ASSIGN:
      if (node->symbol->type == SYMBOL_ARRAY){
        fprintf(stderr, "semantic error: array %s not being indexed\n", node->symbol->text);
      }else if (node->symbol->type == SYMBOL_FUNC){
        fprintf(stderr, "semantic error: trying to assign a value to the function %s\n", node->symbol->text);
      }
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_ARRAY_ASSIGN:
      if (node->symbol->type == SYMBOL_VAR){
        //fprintf(stderr, "semantic error: variable %s being indexed \n", node->symbol->text);
        printError("variable being indexed", "var", node->symbol->text);
      }
      else if (node->symbol->type == SYMBOL_FUNC){
        printError("trying to index and assign a value to a function", "function", node->symbol->text);
      }
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_IF:
    case AST_IF_ELSE:
    case AST_LOOP:
      break;
    case AST_NO_DELIMITER_LIST:
      //fprintf(stderr, "passando por uma lista sem delimitadores \n");
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_COMMA_DELIMITED_LIST:
      //fprintf(stderr, "passando por uma lista separada por , \n");
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_SEMICOLON_DELIMITED_LIST:
      //fprintf(stderr, "passando por uma lista separada por ; \n");
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_RETURN:
    case AST_CMD_BLOCK:
    case AST_PARENTHESES:
      //fprintf(stderr, "passando por return, block ou parenteses\n");
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_FUNC_DECL:
      checkSymbolsUsage(node->sons[2]);
  }
  return;
}

void printError(char *msg, char *type, char *varName){
  fprintf(stderr, "semantic error: %s, %s %s\n", msg, type, varName);
}
