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
#include "tac.h"
#include "assembly.h"

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
  //decompile(global_ast, output);

  int errors = 0;
  errors = fullSemanticCheck(global_ast);
  if (errors) {
    fprintf(stderr, "number of semantic errors: %d\n", errors);
    return 4;
  }

  TAC *taccode = genTac(global_ast);
  tacPrint(taccode);

  fprintf(stderr, "generating assembly...\n");
  if (tacToAssembly(taccode,output)){
    fprintf(stderr, "error occured while generating assembly\n");
    return 6;
  }
  fprintf(stderr, "successfuly generated assembly in file %s\n", argv[2]);

  return 0;
}
