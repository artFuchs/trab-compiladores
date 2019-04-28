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

<<<<<<< HEAD
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
=======
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

>>>>>>> started creating the tree in the parser.y
%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING
<<<<<<< HEAD
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
=======

%token TOKEN_ERROR
>>>>>>> started creating the tree in the parser.y

%type<ast> program;
%type<ast> decl_list;
%type<ast> decl_var;
%type<ast> vector_range;
%type<ast> inivector;
%type<ast> rest_inivector;
%type<ast> decl_func;
%type<ast> param_list;
%type<ast> rest_param_list;
%type<ast> param;
%type<ast> block;
%type<ast> bl_comands;
%type<ast> comand;
%type<ast> print_list;
%type<ast> rest_print_list;
%type<ast> flux_control;
%type<ast> expr;
%type<ast> arg_list;
%type<ast> rest_arg_list;
%type<ast> type;
%type<ast> value;

%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE

%left OPERATOR_OR OPERATOR_AND
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '-' '+'
%left '*' '/'
%right OPERATOR_NOT

%union {
  struct syntax_node *ast;
  struct node *symbol;
  AST *ast;
}

%%

program: decl_list                       {$$ = $1; printTree($$,0); }
       ;

decl_list : decl_var ';' decl_list        {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
          | decl_func ';' decl_list       {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
          |                               {$$ = 0;}
          ;

decl_var: type TK_IDENTIFIER '=' value                       {$$ = newSyntaxNode (0,$2,$1,$4,0,0);}
        | type TK_IDENTIFIER '[' vector_range ']' inivector  {$$ = newSyntaxNode (0,$2,$1,$4,$6,0);}
        ;

<<<<<<< HEAD
inivector: ':' value rest_inivector
        |
        ;
=======
vector_range: LIT_INTEGER                {$$ = newSyntaxNode (0,$1,0,0,0,0);}
            ;

inivector: ':' value rest_inivector       {$$ = newSyntaxNode (0,0,$2,$3,0,0);}
         |                                {$$ = 0;}
         ;
>>>>>>> started creating the tree in the parser.y

rest_inivector: value rest_inivector      {$$ = newSyntaxNode (0,0,$1,$2,0,0);}
              |                           {$$ = 0;}
              ;

<<<<<<< HEAD
decl_func: type TK_IDENTIFIER '(' param_list ')' block     {printf("declaração de funcao\n");}
        ;
=======
decl_func: type TK_IDENTIFIER '(' param_list ')' block     {$$ = newSyntaxNode (0,$2,$1,$4,$6,0);}
         ;
>>>>>>> started creating the tree in the parser.y

param_list: param rest_param_list           {$$ = newSyntaxNode (0,0,$1,$2,0,0);}
          |                                 {$$ = 0;}
          ;

<<<<<<< HEAD
rest_param_list: ',' type TK_IDENTIFIER rest_param_list
          |
          ;
=======
rest_param_list: ',' param rest_param_list   {$$ = newSyntaxNode (0,0,$2,$3,0,0);}
           |                                 {$$ = 0;}
           ;
>>>>>>> started creating the tree in the parser.y

param: type TK_IDENTIFIER                        {$$ = newSyntaxNode (0,$2,$1,0,0,0);}
     ;

<<<<<<< HEAD
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
=======
block: '{' bl_comands '}'                        {$$ = newSyntaxNode (0,0,$2,0,0,0);}
     ;

bl_comands: comand ';' bl_comands                 {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
          |                                       {$$ = 0;}
          ;

comand: TK_IDENTIFIER '=' expr                    {$$ = newSyntaxNode (0,$1,$3,0,0,0);}
      | TK_IDENTIFIER '[' expr ']' '=' expr       {$$ = newSyntaxNode (0,$1,$3,$6,0,0);}
      | flux_control                              {$$ = newSyntaxNode (0,0,$1,0,0,0);}
      | KW_READ TK_IDENTIFIER                     {$$ = newSyntaxNode (0,$2,0,0,0,0);}
      | KW_PRINT print_list                       {$$ = newSyntaxNode (0,0,$2,0,0,0);}
      | KW_RETURN expr                            {$$ = newSyntaxNode (0,0,$2,0,0,0);}
      | block                                     {$$ = newSyntaxNode (0,0,$1,0,0,0);}
      |                                           {$$ = 0;}
>>>>>>> started creating the tree in the parser.y
      ;

print_list: expr rest_print_list                 {$$ = newSyntaxNode (0,0,$1,$2,0,0);}
          ;

<<<<<<< HEAD
rest_print_list: ',' expr rest_print_list
              |
              ;

flux_control: KW_IF '(' expr ')' KW_THEN command   %prec LOWER_THAN_ELSE   {printf ("if then\n");}
            | KW_IF '(' expr ')' KW_THEN command KW_ELSE command            {printf ("if then else\n");}
            | KW_LOOP '(' expr ')' command                                 {printf ("loop\n");}
            | KW_LEAP                                                     {printf ("leap\n");}
=======
rest_print_list: ',' expr rest_print_list         {$$ = newSyntaxNode (0,0,$2,$3,0,0);}
               |                                  {$$ = 0;}
               ;

flux_control: KW_IF '(' expr ')' KW_THEN comand   %prec LOWER_THAN_ELSE   {$$ = newSyntaxNode (0,0,$3,$6,0,0);}
            | KW_IF '(' expr ')' KW_THEN comand KW_ELSE comand            {$$ = newSyntaxNode (0,0,$3,$6,$8,0);}
            | KW_LOOP '(' expr ')' comand                                 {$$ = newSyntaxNode (0,0,$3,$5,0,0);}
            | KW_LEAP                                                     {$$ = newSyntaxNode (0,0,0,0,0,0);}
>>>>>>> started creating the tree in the parser.y
            ;

expr: TK_IDENTIFIER                   {$$ = newSyntaxNode (0,$1,0,0,0,0);}
    | TK_IDENTIFIER '[' expr ']'      {$$ = newSyntaxNode (0,$1,$3,0,0,0);}
    | value                           {$$ = newSyntaxNode (0,0,$1,0,0,0);}
    | TK_IDENTIFIER '(' arg_list ')'  {$$ = newSyntaxNode (0,$1,$3,0,0,0);}
    | expr '+' expr                   {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr '-' expr                   {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr '*' expr                   {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr '/' expr                   {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr '<' expr                   {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr '>' expr                   {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr OPERATOR_AND expr          {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr OPERATOR_OR expr           {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | OPERATOR_NOT expr               {$$ = newSyntaxNode (0,0,$2,0,0,0);}
    | expr OPERATOR_LE expr           {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr OPERATOR_GE expr           {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr OPERATOR_EQ expr           {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | expr OPERATOR_DIF expr          {$$ = newSyntaxNode (0,0,$1,$3,0,0);}
    | '(' expr ')'                    {$$ = newSyntaxNode (0,0,$2,0,0,0);}
    ;

arg_list: expr rest_arg_list           {$$ = newSyntaxNode (0,0,$1,$2,0,0);}
        ;

<<<<<<< HEAD
rest_arg_list: ',' expr rest_arg_list
            |
            ;
=======
rest_arg_list: ',' expr rest_arg_list  {$$ = newSyntaxNode (0,0,$2,$3,0,0);}
             |                         {$$ = 0;}
             ;
>>>>>>> started creating the tree in the parser.y

type: KW_INT              {$$ = newSyntaxNode (0,0,0,0,0,0);}
    | KW_FLOAT            {$$ = newSyntaxNode (0,0,0,0,0,0);}
    | KW_BYTE             {$$ = newSyntaxNode (0,0,0,0,0,0);}
    ;

<<<<<<< HEAD
value: LIT_INTEGER
    | LIT_FLOAT
    | LIT_CHAR
    | LIT_STRING
    ;
=======
value: LIT_INTEGER        {$$ = newSyntaxNode (0,$1,0,0,0,0);}
     | LIT_FLOAT          {$$ = newSyntaxNode (0,$1,0,0,0,0);}
     | LIT_CHAR           {$$ = newSyntaxNode (0,$1,0,0,0,0);}
     | LIT_STRING         {$$ = newSyntaxNode (0,$1,0,0,0,0);}
     ;
>>>>>>> started creating the tree in the parser.y


%%

void yyerror (char *s){
  fprintf(stderr, "Line %d: %s\n", getLineNumber(), s);
  exit(3);
}
