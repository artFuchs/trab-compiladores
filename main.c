#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "hash.h"

//lex.yy.h
int yylex();
extern char *yytext;

int isRunning();

int main() {
  hashInit();
  while (isRunning()){
    int token = yylex();
    switch (token){
      case KW_BYTE:
        printf ("KW_BYTE ");
        break;
      case KW_INT :
        printf ("KW_INT ");
        break;
      case KW_FLOAT :
        printf ("KW_FLOAT ");
        break;
      case KW_IF :
        printf ("KW_IF ");
        break;
      case KW_THEN :
        printf ("KW_THEN ");
        break;
      case KW_ELSE :
        printf ("KW_ELSE ");
        break;
      case KW_LOOP :
        printf ("KW_LOOP ");
        break;
      case KW_LEAP :
        printf ("KW_LEAP ");
        break;
      case KW_READ :
        printf ("KW_READ ");
        break;
      case KW_RETURN :
        printf ("KW_RETURN ");
        break;
      case KW_PRINT :
        printf ("KW_PRINT ");
        break;
      case OPERATOR_LE :
        printf ("<= ");
        break;
      case OPERATOR_GE :
        printf (">= ");
        break;
      case OPERATOR_EQ :
        printf ("== ");
        break;
      case OPERATOR_DIF :
        printf ("!= ");
        break;
      case OPERATOR_AND :
        printf ("&& ");
        break;
      case OPERATOR_OR :
        printf ("|| ");
        break;
      case TK_IDENTIFIER :
        printf ("TK_IDENTIFIER: %s ", yytext);
        break;
      case LIT_INTEGER:
        printf ("LIT_INTEGER: %s ", yytext);
        break;
      case LIT_FLOAT:
        printf ("LIT_FLOAT: %s ", yytext);
        break;
      case LIT_CHAR:
        printf ("LIT_CHAR: %s ", yytext);
        break;
      case LIT_STRING:
        printf ("LIT_STRING: %s ", yytext);
        break;
      case TOKEN_ERROR :
        printf ("ERROR ");
        break;
      default :
        printf ("%c ", (char) token);
    }
  }
  return 0;
}
