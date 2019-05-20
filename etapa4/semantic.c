#include "semantic.h"
#include "hash.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

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
