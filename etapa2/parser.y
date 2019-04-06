%{
  /******************************************************
  *                                                     *
  * Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
  *                                                     *
  ******************************************************/
  #include <stdio.h>
  int yylex(void);
  void yyerror(char *);
%}

%token KW_BYTE
%token KW_INT
%token KW_FLOAT
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_LOOP
%token KW_LEAP
%token KW_READ
%token KW_RETURN
%token KW_PRINT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF
%token OPERATOR_OR
%token OPERATOR_AND
%token OPERATOR_NOT
%token TK_IDENTIFIER
%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_CHAR
%token LIT_STRING
%token TOKEN_ERROR

%%

program: declarations ;

declarations: decl_var ';' declarations     {printf("declarações\n");}
            |                               {printf("vazio\n");}
            ;

decl_var: type TK_IDENTIFIER '=' value
        | type TK_IDENTIFIER '[' LIT_INTEGER ']' inivector

inivector: ':' value rest_inivector
         |
         ;

rest_inivector: ' ' value rest_inivector
              |
              ;

type: KW_INT
    | KW_FLOAT
    | KW_BYTE
    ;

value: LIT_INTEGER
     | LIT_FLOAT
     | LIT_CHAR
     | LIT_STRING
     ;


%%

void yyerror (char *s){
  fprintf(stderr, "%s\n", s);
}

int main(){
  yyparse();
  return 0;
}
