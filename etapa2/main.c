#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "y.tab.h"

void initMe();


int main(){
  initMe();
  yyparse();
  printTable();
  return 0;
}
