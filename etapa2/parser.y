%{
  /******************************************************
  *                                                     *
  * Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
  *                                                     *
  ******************************************************/
  #include <stdio.h>
  #include <stdlib.h>
  int yylex(void);
  void yyerror(char *);
  int getLineNumber();
  void initMe();
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

%left OPERATOR_OR OPERATOR_AND
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '-' '+'
%left '*' '/'
%right OPERATOR_NOT

%%

program: decl_list ;

decl_list: decl_var ';' decl_list
            | decl_func ';' decl_list
            |
            ;

decl_var: type TK_IDENTIFIER '=' value                      {printf("declaracao de variavel\n");}
        | type TK_IDENTIFIER '[' LIT_INTEGER ']' inivector  {printf("declaracao de vetor\n");}
        ;

inivector: ':' value rest_inivector
         |
         ;

rest_inivector: value rest_inivector
              |
              ;

decl_func: type TK_IDENTIFIER '(' param_list ')' block     {printf("declaração de funcao\n");}
         ;

param_list: param rest_param_list
          |
          ;

rest_param_list: ',' type TK_IDENTIFIER rest_param_list
           |
           ;

param: type TK_IDENTIFIER;

block: '{' bl_comands '}';

bl_comands: comand ';' bl_comands
          | comand
          |
          ;

comand: TK_IDENTIFIER '=' expr
      | TK_IDENTIFIER '[' expr ']' '=' expr
      | flux_control
      | KW_READ TK_IDENTIFIER
      | KW_PRINT print_list
      | KW_RETURN expr
      | block
      |
      ;

print_list: LIT_STRING rest_print_list
          | expr rest_print_list
          ;

rest_print_list: ',' LIT_STRING rest_print_list
               | ',' expr rest_print_list
               |
               ;

flux_control: KW_IF '(' expr ')' KW_THEN comand KW_ELSE comand      {printf ("if then else\n");}
            | KW_IF '(' expr ')' KW_THEN comand                     {printf ("if then\n");}
            | KW_LOOP '(' expr ')' comand                           {printf ("loop\n");}
            | KW_LEAP                                               {printf ("leap\n");}
            ;

expr: TK_IDENTIFIER
    | TK_IDENTIFIER '[' expr ']'
    | value
    | TK_IDENTIFIER '(' arg_list ')'
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | expr OPERATOR_AND expr
    | expr OPERATOR_OR expr
    | expr OPERATOR_NOT expr
    | expr OPERATOR_LE expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_DIF expr
    ;

arg_list: expr rest_arg_list

rest_arg_list: ',' expr rest_arg_list
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
  fprintf(stderr, "Line %d: %s\n", getLineNumber(), s);
  exit(3);
}

int main(){
  initMe();
  yyparse();
  return 0;
}
