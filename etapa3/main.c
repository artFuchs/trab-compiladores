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

void initMe();
extern FILE *yyin;

int main(int argc, char *argv[]){
  if(argc < 2){
    fprintf(stderr, "%s", "Missing file name! Command format: ./etapa2 <file_name> \n");
    exit(1);
  }

  initMe();

  FILE* file = fopen(argv[1], "r");
  if(file == NULL){
    fprintf(stderr, "%s", "Can't open file. \n");
    exit(1);
  }

  yyin = file;
  yyparse();

  printTable();

  return 0;
}
