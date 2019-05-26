#include "semantic.h"
#include "hash.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

void printError(char *msg, char *type, char *varName);
int getFunctionCallParamCount(AST *node);

void declareSymbol(AST *node, int type){
  if (node->symbol->type == SYMBOL_FUNC){
    if (node->sons[0]->symbol->type != SYMBOL_IDENTIFIER){
      printError("symbol redeclared","param", node->symbol->text);
      return;
    }
  }
  else if (node->symbol->type != SYMBOL_IDENTIFIER){
    fprintf (stderr,"semantic error: variable %s redeclared\n", node->symbol->text);
    switch (node->symbol->type){
      case SYMBOL_VAR:
        printError("symbol redeclared","variable", node->symbol->text);
        break;
      case SYMBOL_ARRAY:
        printError("symbol redeclared","array", node->symbol->text);
        break;
      case SYMBOL_FUNC:
        printError("symbol redeclared","function", node->symbol->text);
        break;
    }

    return;
  }

  switch (type){
    case AST_PARAM_ELEM:
      node->symbol->type = SYMBOL_VAR;
      node->symbol->n_params=0;
      // TODO: setar tipo do parametro
      break;
    case AST_VAR_DECL:
      node->symbol->type = SYMBOL_VAR;
      node->symbol->n_params=0;
      // TODO: setar tipo da variavel
      break;
    case AST_ARRAY_DECL:
      node->symbol->type = SYMBOL_ARRAY;
      node->symbol->n_params=0;
      // TODO: setar tipo do array
      break;
    case AST_FUNC_DECL:
      node->symbol->type = SYMBOL_FUNC;
      node->symbol->n_params=getFunctionCallParamCount(node->sons[1]);
      printf("function %s has %d params\n", node->symbol->text, node->symbol->n_params);
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
        printError("symbol not declared","", node->text);
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
          printError("trying to index a variable","variable", node->symbol->text);
        }
      }
      else if (node->symbol->type == SYMBOL_ARRAY){
        if (node->sons[0]==0)
          printError("array not being indexed", "array", node->symbol->text );
        else
          checkSymbolsUsage(node->sons[0]);
      }
      else if (node->symbol->type == SYMBOL_FUNC){
        printError("trying to use function as variable", "function", node->symbol->text);
      }
      break;
    case AST_FUNC_CALL:
      if (node->symbol->type != SYMBOL_FUNC)
        printError("variable being used as function", "variable", node->symbol->text);
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
        printError("array not being indexed\n", "array", node->symbol->text);
      }else if (node->symbol->type == SYMBOL_FUNC){
        printError("trying to assign value to a function", "function", node->symbol->text);
      }
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_ARRAY_ASSIGN:
      if (node->symbol->type == SYMBOL_VAR){
        printError("variable being indexed", "var", node->symbol->text);
      }
      else if (node->symbol->type == SYMBOL_FUNC){
        printError("trying to index and assign a value to a function", "function", node->symbol->text);
      }
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_IF:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_IF_ELSE:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      checkSymbolsUsage(node->sons[2]);
      break;
    case AST_LOOP:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_NO_DELIMITER_LIST:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_COMMA_DELIMITED_LIST:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_SEMICOLON_DELIMITED_LIST:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_READ:
      if (node->symbol->type != SYMBOL_VAR){
        char *msg = "invalid argument to the keyword READ";
        switch (node->symbol->type){
          case SYMBOL_FUNC:
            printError(msg,"function",node->symbol->text);
          case SYMBOL_ARRAY:
            printError(msg,"array",node->symbol->text);
          default:
            printError(msg,"unknown symbol",node->symbol->text);
        }
      }
      break;
    case AST_PRINT:
    case AST_RETURN:
    case AST_CMD_BLOCK:
    case AST_PARENTHESES:
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_FUNC_DECL:
      checkSymbolsUsage(node->sons[2]);
  }
  return;
}

void checkFunctionCallParams(AST *node){
  if (!node) return;
  if (node->type==AST_FUNC_CALL){
    int count = getFunctionCallParamCount(node->sons[0]);
    if (count!=node->symbol->n_params){
      printError("wrong number of params in function call", "function", node->symbol->text);
      fprintf(stderr, "expected: %d / got: %d\n", node->symbol->n_params, count);
    }
  }
  int i;
  for (i=0;i<MAX_SONS;i++){
    checkFunctionCallParams(node->sons[i]);
  }
}

int getFunctionCallParamCount(AST *node){
  if (!node) return 0;
  int count=0;
  while (node){
    node = node->sons[1];
    count++;
  }
  return count;
}

void printError(char *msg, char *type, char *varName){
  fprintf(stderr, "semantic error: %s, %s %s\n", msg, type, varName);
}
