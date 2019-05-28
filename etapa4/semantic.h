/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

void declareSymbol(AST *node, int type);
void hashCheckUndeclared();
void checkSymbolsUsage(AST *node);
void setDataType(AST *node, int type);
void checkDataType(AST *node);
void checkFunctionCallParams(AST *node);
#endif
