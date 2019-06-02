#include "semantic.h"
#include "hash.h"
#include "symbols.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void printError(char *msg, char *type, char *varName, int lineNumber);
int getFunctionCallParamCount(AST *node);
int typeInference(int, int);
void compareFunctionParams(AST*, AST*);

int semantic_errors = 0;

int fullSemanticCheck(AST *node){
  printf("checking undeclared symbols in the hash...\n");
  hashCheckUndeclared();
  printf("checking symbol's usage...\n");
  checkSymbolsUsage(global_ast);
  printf("checking types...\n");
  checkDataType(global_ast);
  printf("checking function arguments...\n");
  checkFunctionCallParams(global_ast);
  return semantic_errors;
}

void declareSymbol(AST *node, int type) {
  if (node->symbol->type == SYMBOL_FUNC){
    if (node->sons[0]->symbol->type != SYMBOL_IDENTIFIER){
      printError("symbol redeclared","param", node->symbol->text, node->lineNumber);
      return;
    }
  }
  else if (node->symbol->type != SYMBOL_IDENTIFIER) {
    printError("symbol redeclared", "symbol", node->symbol->text, node->lineNumber);
    switch (node->symbol->type){
      case SYMBOL_VAR:
        printError("symbol redeclared","variable", node->symbol->text, node->lineNumber);
        break;
      case SYMBOL_ARRAY:
        printError("symbol redeclared","array", node->symbol->text, node->lineNumber);
        break;
      case SYMBOL_FUNC:
        printError("symbol redeclared","function", node->symbol->text, node->lineNumber);
        break;
    }
    return;
  }

  switch (type){
    case AST_PARAM_ELEM:
      node->symbol->type = SYMBOL_VAR;
      setDataType(node, node->sons[0]->type);
      node->symbol->n_params=0;
      break;
    case AST_VAR_DECL:
      node->symbol->type = SYMBOL_VAR;
      setDataType(node, node->sons[0]->type);
      node->symbol->n_params=0;
      break;
    case AST_ARRAY_DECL:
      node->symbol->type = SYMBOL_ARRAY;
			setDataType(node, node->sons[0]->type);
      node->symbol->n_params=0;
      break;
    case AST_FUNC_DECL:
      node->symbol->type = SYMBOL_FUNC;
			setDataType(node, node->sons[0]->type);
      node->symbol->n_params=getFunctionCallParamCount(node->sons[1]);
      node->symbol->ast_link=node;
      printf("function %s has %d params\n", node->symbol->text, node->symbol->n_params);

      break;
  }
}

void setDataType(AST *node, int type) {
  switch (type) {
    case AST_TINT:
      node->symbol->dataType = DATATYPE_INT;
      break;
    case AST_TFLOAT:
      node->symbol->dataType = DATATYPE_FLOAT;
      break;
    case AST_TBYTE:
      node->symbol->dataType = DATATYPE_BYTE;
      break;
    default:
      node->symbol->dataType = DATATYPE_UNDEFINED;
  }
  node->dataType = node->symbol->dataType;
}

void hashCheckUndeclared() {
  int i;
  for (i=0; i<HASH_SIZE; i++){
    NODE *node;
    for (node = Table[i]; node; node=node->next){
      if (node->type == SYMBOL_IDENTIFIER){
        printError("symbol not declared","", node->text, node->lineNumber);
      }
    }
  }
  return;
}

void checkSymbolsUsage(AST *node) {
  if (node == 0) return;
  switch (node->type) {
    case AST_SYMBOL:
      if (node->symbol->type == SYMBOL_VAR) {
        if (node->sons[0]){
          printError("trying to index a variable","variable", node->symbol->text, node->lineNumber);
        }
      }
      else if (node->symbol->type == SYMBOL_ARRAY) {
        if (node->sons[0]==0)
          printError("array not being indexed", "array", node->symbol->text, node->lineNumber);
        else
          checkSymbolsUsage(node->sons[0]);
      }
      else if (node->symbol->type == SYMBOL_FUNC) {
        printError("trying to use function as variable", "function", node->symbol->text, node->lineNumber);
      }
      break;
    case AST_FUNC_CALL:
      if (node->symbol->type != SYMBOL_FUNC)
        printError("variable being used as function", "variable", node->symbol->text, node->lineNumber);
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
    case AST_NEQ:
      checkSymbolsUsage(node->sons[0]);
      checkSymbolsUsage(node->sons[1]);
      break;
    case AST_NOT:
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_ASSIGN:
      if (node->symbol->type == SYMBOL_ARRAY){
        printError("array not being indexed\n", "array", node->symbol->text, node->lineNumber);
      }else if (node->symbol->type == SYMBOL_FUNC){
        printError("trying to assign value to a function", "function", node->symbol->text, node->lineNumber);
      }
      checkSymbolsUsage(node->sons[0]);
      break;
    case AST_ARRAY_ASSIGN:
      if (node->symbol->type == SYMBOL_VAR){
        printError("variable being indexed", "var", node->symbol->text, node->lineNumber);
      }
      else if (node->symbol->type == SYMBOL_FUNC){
        printError("trying to index and assign a value to a function", "function", node->symbol->text, node->lineNumber);
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
        printError("invalid argument to the keyword READ", node->symbol->text, NULL, node->lineNumber);
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


void checkDataType(AST *node) {
  if (node == NULL) return;

  for (int i=0; i < MAX_SONS; i++) {
    checkDataType(node->sons[i]);
  }

  switch (node->type) {
    case AST_SYMBOL:
      if (node->symbol->type == SYMBOL_ARRAY && node->sons[0]==0) {
        printError("Invalid use of array", "symbol", node->symbol->text, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else if (node->symbol->type == SYMBOL_FUNC) {
        printError("Invalid use of function", "symbol", node->symbol->text, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else {
        node->dataType = node->symbol->dataType;
      }
      break;
    case AST_ASSIGN:
      if (node->sons[0]->dataType == DATATYPE_UNDEFINED) {
        printError("Can't assign undefined expression", "assignment", node->symbol->text, node->lineNumber);
      }
      else{
        node->dataType = typeInference(node->symbol->dataType, node->sons[0]->dataType);
        if (node->dataType == DATATYPE_UNDEFINED) {
          printError("Type conflict on assignment", "assignment", node->symbol->text, node->lineNumber);
        }
      }
      break;
    case AST_LT:
    case AST_LE:
    case AST_GT:
    case AST_GE:
      if (node->sons[0]->dataType == DATATYPE_BOOL || node->sons[1]->dataType == DATATYPE_BOOL) {
        printError("Can't compare boolean expression", "comparison", NULL, node->lineNumber);
      }
      else if (node->sons[0]->dataType == DATATYPE_UNDEFINED || node->sons[1]->dataType == DATATYPE_UNDEFINED) {
        printError("Can't compare undefined expressions", "comparison", NULL, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else {
        node->dataType = DATATYPE_BOOL;
      }
      break;
    case AST_EQ:
    case AST_NEQ:
      if (node->sons[0]->dataType == DATATYPE_BOOL || node->sons[1]->dataType == DATATYPE_BOOL) {
        printError("Can't compare booleans", "equality", NULL, node->lineNumber);
      }
      else if (node->sons[0]->dataType == DATATYPE_STRING || node->sons[1]->dataType == DATATYPE_STRING) {
        printError("Can't compare strings", "equality", NULL, node->lineNumber);
      }
      else if (node->sons[0]->dataType == DATATYPE_UNDEFINED || node->sons[1]->dataType == DATATYPE_UNDEFINED) {
        printError("Can't compare undefined expressions", "equality", NULL, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else {
        node->dataType = DATATYPE_BOOL;
      }
      break;
    case AST_AND:
    case AST_OR:
      if (node->sons[0]->dataType != DATATYPE_BOOL || node->sons[1]->dataType != DATATYPE_BOOL) {
        printError("Expected boolean expressions", "logic expr", NULL, node->lineNumber);
      }
      else if (node->sons[0]->dataType == DATATYPE_UNDEFINED || node->sons[1]->dataType == DATATYPE_UNDEFINED) {
        printError("Unexpected undefined expression", "logic expr", NULL, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else {
        node->dataType = DATATYPE_BOOL;
      }
    case AST_NOT:
      if (node->sons[0]->dataType == DATATYPE_UNDEFINED) {
        printError("Can't negate an undefined expression", "negation", node->sons[0]->symbol->text, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else if (node->sons[0]->dataType != DATATYPE_BOOL) {
        printError("Can't negate something that's not a boolean expression", "negation", node->sons[0]->symbol->text, node->lineNumber);
      }
      else {
        node->dataType = DATATYPE_BOOL;
      }
      break;
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
      if (node->sons[0]->dataType==DATATYPE_BOOL || node->sons[1]->dataType==DATATYPE_BOOL){
        printError("unexpected boolean expression inside aritmetic expression", NULL, NULL, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else if (node->sons[0]->dataType==DATATYPE_UNDEFINED || node->sons[1]->dataType==DATATYPE_UNDEFINED){
        printError("couldn't assert aritmétic expression value", NULL, NULL, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else if (node->sons[0]->dataType==DATATYPE_STRING || node->sons[1]->dataType==DATATYPE_STRING){
        printError("unexpected string inside aritmetic expression", NULL, NULL, node->lineNumber);
        node->dataType = DATATYPE_UNDEFINED;
      }
      else{
        node->dataType = typeInference(node->sons[0]->dataType, node->sons[1]->dataType);
      }
      break;
    case AST_INTEGER:
      node->dataType = DATATYPE_INT;
      break;
    case AST_FLOAT:
      node->dataType = DATATYPE_FLOAT;
      break;
    case AST_CHAR:
      node->dataType = DATATYPE_CHAR;
      break;
    case AST_STRING:
      node->dataType = DATATYPE_STRING;
      break;
    case AST_RETURN:
      if (node->sons[0]->dataType==DATATYPE_BOOL)
        printError("boolean expression after keyword return", NULL, NULL, node->lineNumber);
      else if (node->sons[0]->dataType==DATATYPE_STRING)
        printError("string after keyword return", NULL, NULL, node->lineNumber);
      else if (node->sons[0]->dataType==DATATYPE_UNDEFINED)
        printError("couldn't assert the type of the expresion after keyword return", NULL, NULL, node->lineNumber);
      break;
    case AST_IF:
    case AST_IF_ELSE:
      if (node->sons[0]->dataType!=DATATYPE_BOOL){
        printError("if case is not an boolean expression", NULL, NULL, node->lineNumber);
      }
      break;
    case AST_PARENTHESES:
      node->dataType = node->sons[0]->dataType;
      break;
  }
}

int typeInference(int type1, int type2){
  if (type1 == DATATYPE_FLOAT || type2 == DATATYPE_FLOAT){
    return DATATYPE_FLOAT;
  }
  else if (type1 == DATATYPE_INT || type2 == DATATYPE_INT){
    return DATATYPE_INT;
  }
  else if (type1 == DATATYPE_BYTE || type2 == DATATYPE_BYTE){
    return DATATYPE_BYTE;
  }
  else if (type1 == DATATYPE_CHAR || type2 == DATATYPE_CHAR){
    return DATATYPE_CHAR;
  }
  else{
    return DATATYPE_UNDEFINED;
  }
  return DATATYPE_UNDEFINED;
}

void checkFunctionCallParams(AST *node){
  if (!node) return;
  if (node->type==AST_FUNC_CALL){
    int count = getFunctionCallParamCount(node->sons[0]);
    if (count!=node->symbol->n_params){
      printError("wrong number of params in function call", "function", node->symbol->text, node->lineNumber);
      fprintf(stderr, "expected: %d / got: %d\n", node->symbol->n_params, count);
    }
    compareFunctionParams(node->sons[1],node->symbol->ast_link->sons[1]);
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

void compareFunctionParams(AST *node, AST *original_node){
  if (!node) return;
  if (!original_node) return;
  if (node->symbol->dataType != original_node->symbol->dataType){
    printError("param type does not mathc with the requested type", node->symbol->text, original_node->symbol->text, node->lineNumber);
  }
  compareFunctionParams(node->sons[1], original_node->sons[1]);
}

void printError(char *msg, char *type, char *varName, int lineNumber){
  semantic_errors+=1;
  if (varName) {
    fprintf(stderr, "semantic error: %s, %s %s on line %d\n", msg, type, varName, lineNumber);
  }
  else
    fprintf(stderr, "semantic error: %s, %s on line %d\n", msg, type, lineNumber);
}
