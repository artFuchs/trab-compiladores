/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "y.tab.h"
#include "ast.h"
#include "semantic.h"

void initMe();
extern FILE *yyin;

int main(int argc, char *argv[]){
  if(argc < 3){
    fprintf(stderr, "%s", "You're wrong! Command format: ./etapa2 <input> <output> \n");
    exit(1);
  }

  initMe();

  FILE* input = fopen(argv[1], "r");
  if(!input){
    fprintf(stderr, "%s", "Can't open input file. \n");
    exit(1);
  }
  FILE* output = fopen(argv[2], "w+");
  if(!output){
    fprintf(stderr, "%s", "Can't open output file. \n");
    exit(1);
  }

  yyin = input;
  yyparse();


  printTable();
  decompile(global_ast, output);

  printf("checking undeclared symbols in the hash...\n");
  hashCheckUndeclared();
  printf("checking symbol's usage...\n");
  checkSymbolsUsage(global_ast);
  checkFunctionCallParams(global_ast);

  return 0;
}
