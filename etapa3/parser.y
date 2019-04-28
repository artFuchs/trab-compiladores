%{
  /******************************************************
  *                                                     *
  * Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
  *                                                     *
  ******************************************************/
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
  int yylex(void);
  void yyerror(char *);
  int getLineNumber();
  void initMe();
%}

%token<symbol> KW_BYTE
%token<symbol> KW_INT
%token<symbol> KW_FLOAT
%token<symbol> KW_IF
%token<symbol> KW_THEN
%token<symbol> KW_ELSE
%token<symbol> KW_LOOP
%token<symbol> KW_LEAP
%token<symbol> KW_READ
%token<symbol> KW_RETURN
%token<symbol> KW_PRINT
%token<symbol> OPERATOR_LE
%token<symbol> OPERATOR_GE
%token<symbol> OPERATOR_EQ
%token<symbol> OPERATOR_DIF
%token<symbol> OPERATOR_OR
%token<symbol> OPERATOR_AND
%token<symbol> OPERATOR_NOT
%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING
%token<symbol> TOKEN_ERROR

%type<ast> program
%type<ast> decl_var
%type<ast> inivector
%type<ast> rest_inivector
%type<ast> decl_func
%type<ast> param_list
%type<ast> rest_param_list
%type<ast> param
%type<ast> block
%type<ast> bl_commands
%type<ast> command
%type<ast> print_list
%type<ast> rest_print_list
%type<ast> flux_control
%type<ast> expr
%type<ast> arg_list
%type<ast> rest_arg_list
%type<ast> type

%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE

%left OPERATOR_OR OPERATOR_AND
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '-' '+'
%left '*' '/'
%right OPERATOR_NOT

%union {
  struct node *symbol;
  AST *ast;
}

%%

program: decl_list;

decl_list : decl_var ';' decl_list
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

block: '{' bl_commands '}';

bl_commands: command ';' bl_commands
          |
          ;

command: TK_IDENTIFIER '=' expr
      | TK_IDENTIFIER '[' expr ']' '=' expr
      | flux_control
      | KW_READ TK_IDENTIFIER
      | KW_PRINT print_list
      | KW_RETURN expr
      | block
      |
      ;

// if (asd) then print l0 l1 l2 else cmd ;

print_list: expr rest_print_list
          ;

rest_print_list: ',' expr rest_print_list
              |
              ;

flux_control: KW_IF '(' expr ')' KW_THEN command   %prec LOWER_THAN_ELSE   {printf ("if then\n");}
            | KW_IF '(' expr ')' KW_THEN command KW_ELSE command            {printf ("if then else\n");}
            | KW_LOOP '(' expr ')' command                                 {printf ("loop\n");}
            | KW_LEAP                                                     {printf ("leap\n");}
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
    | '(' expr ')'
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
