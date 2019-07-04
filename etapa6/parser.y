%{
  /******************************************************
  *                                                     *
  * Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
  *                                                     *
  ******************************************************/
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
  #include "semantic.h"
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

%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING
%token TOKEN_ERROR

%type<ast> program
%type<ast> decl_list
%type<ast> decl_var
%type<ast> vector_range
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
%type<ast> value

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
}

%%

program: decl_list                       {$$ = $1; printTree($$,0); global_ast = $$;}
       ;

decl_list : decl_var ';' decl_list        {$$ = newSyntaxNode (AST_SEMICOLON_DELIMITED_LIST,0,$1,$3,0,0,getLineNumber());}
          | decl_func ';' decl_list       {$$ = newSyntaxNode (AST_SEMICOLON_DELIMITED_LIST,0,$1,$3,0,0,getLineNumber());}
          |                               {$$ = 0;}
          ;

decl_var: type TK_IDENTIFIER '=' value                       {$$ = newSyntaxNode (AST_VAR_DECL,$2,$1,$4,0,0,getLineNumber()); declareSymbol($$,AST_VAR_DECL);}
        | type TK_IDENTIFIER '[' vector_range ']' inivector  {$$ = newSyntaxNode (AST_ARRAY_DECL,$2,$1,$4,$6,0,getLineNumber()); declareSymbol($$,AST_ARRAY_DECL);}
        ;

vector_range: LIT_INTEGER                {$$ = newSyntaxNode (AST_INTEGER,$1,0,0,0,0,getLineNumber());}
            ;

inivector: ':' value rest_inivector       {$$ = newSyntaxNode (AST_NO_DELIMITER_LIST,0,$2,$3,0,0,getLineNumber());}
         |                                {$$ = 0;}
         ;

rest_inivector: value rest_inivector      {$$ = newSyntaxNode (AST_NO_DELIMITER_LIST,0,$1,$2,0,0,getLineNumber());}
              |                           {$$ = 0;}
              ;

decl_func: type TK_IDENTIFIER '(' param_list ')' block     {$$ = newSyntaxNode (AST_FUNC_DECL,$2,$1,$4,$6,0,getLineNumber()); declareSymbol ($$, AST_FUNC_DECL);}
         ;

param_list: param rest_param_list           {$$ = newSyntaxNode (AST_NO_DELIMITER_LIST,0,$1,$2,0,0,getLineNumber());}
          |                                 {$$ = 0;}
          ;

rest_param_list: ',' param rest_param_list   {$$ = newSyntaxNode (AST_COMMA_DELIMITED_LIST,0,$2,$3,0,0,getLineNumber());}
           |                                 {$$ = 0;}
           ;

param: type TK_IDENTIFIER                        {$$ = newSyntaxNode (AST_PARAM_ELEM,$2,$1,0,0,0,getLineNumber()); declareSymbol($$,AST_PARAM_ELEM);}
     ;

block: '{' bl_commands '}'                        {$$ = newSyntaxNode (AST_CMD_BLOCK,0,$2,0,0,0,getLineNumber());}
     ;

bl_commands: command ';' bl_commands                {$$ = newSyntaxNode (AST_SEMICOLON_DELIMITED_LIST,0,$1,$3,0,0,getLineNumber());}
          |                                       {$$ = 0;}
          ;

command: TK_IDENTIFIER '=' expr                   {$$ = newSyntaxNode (AST_ASSIGN,$1,$3,0,0,0,getLineNumber());}
      | TK_IDENTIFIER '[' expr ']' '=' expr       {$$ = newSyntaxNode (AST_ARRAY_ASSIGN,$1,$3,$6,0,0,getLineNumber());}
      | flux_control                              {$$ = $1;}
      | KW_READ TK_IDENTIFIER                     {$$ = newSyntaxNode (AST_READ,$2,0,0,0,0,getLineNumber());}
      | KW_PRINT print_list                       {$$ = newSyntaxNode (AST_PRINT,0,$2,0,0,0,getLineNumber());}
      | KW_RETURN expr                            {$$ = newSyntaxNode (AST_RETURN,0,$2,0,0,0,getLineNumber());}
      | block                                     {$$ = $1;}
      |                                           {$$ = 0;}
      ;

print_list: expr rest_print_list                 {$$ = newSyntaxNode (AST_PRINT_LIST,0,$1,$2,0,0,getLineNumber());}
          ;

rest_print_list: ',' expr rest_print_list         {$$ = newSyntaxNode (AST_PRINT_LIST,0,$2,$3,0,0,getLineNumber());}
               |                                  {$$ = 0;}
               ;

flux_control: KW_IF '(' expr ')' KW_THEN command   %prec LOWER_THAN_ELSE   {$$ = newSyntaxNode (AST_IF,0,$3,$6,0,0,getLineNumber());}
            | KW_IF '(' expr ')' KW_THEN command KW_ELSE command            {$$ = newSyntaxNode (AST_IF_ELSE,0,$3,$6,$8,0,getLineNumber());}
            | KW_LOOP '(' expr ')' command                                 {$$ = newSyntaxNode (AST_LOOP,0,$3,$5,0,0,getLineNumber());}
            | KW_LEAP                                                     {$$ = newSyntaxNode (AST_LEAP,0,0,0,0,0,getLineNumber());}
            ;

expr: TK_IDENTIFIER                   {$$ = newSyntaxNode (AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
    | TK_IDENTIFIER '[' expr ']'      {$$ = newSyntaxNode (AST_SYMBOL,$1,$3,0,0,0,getLineNumber());}
    | value                           {$$ = $1;}
    | TK_IDENTIFIER '(' arg_list ')'  {$$ = newSyntaxNode (AST_FUNC_CALL,$1,$3,0,0,0,getLineNumber());}
    | expr '+' expr                   {$$ = newSyntaxNode (AST_ADD,0,$1,$3,0,0,getLineNumber());}
    | expr '-' expr                   {$$ = newSyntaxNode (AST_SUB,0,$1,$3,0,0,getLineNumber());}
    | expr '*' expr                   {$$ = newSyntaxNode (AST_MUL,0,$1,$3,0,0,getLineNumber());}
    | expr '/' expr                   {$$ = newSyntaxNode (AST_DIV,0,$1,$3,0,0,getLineNumber());}
    | expr '<' expr                   {$$ = newSyntaxNode (AST_LT,0,$1,$3,0,0,getLineNumber());}
    | expr '>' expr                   {$$ = newSyntaxNode (AST_GT,0,$1,$3,0,0,getLineNumber());}
    | expr OPERATOR_AND expr          {$$ = newSyntaxNode (AST_AND,0,$1,$3,0,0,getLineNumber());}
    | expr OPERATOR_OR expr           {$$ = newSyntaxNode (AST_OR,0,$1,$3,0,0,getLineNumber());}
    | OPERATOR_NOT expr               {$$ = newSyntaxNode (AST_NOT,0,$2,0,0,0,getLineNumber());}
    | expr OPERATOR_LE expr           {$$ = newSyntaxNode (AST_LE,0,$1,$3,0,0,getLineNumber());}
    | expr OPERATOR_GE expr           {$$ = newSyntaxNode (AST_GE,0,$1,$3,0,0,getLineNumber());}
    | expr OPERATOR_EQ expr           {$$ = newSyntaxNode (AST_EQ,0,$1,$3,0,0,getLineNumber());}
    | expr OPERATOR_DIF expr          {$$ = newSyntaxNode (AST_NEQ,0,$1,$3,0,0,getLineNumber());}
    | '(' expr ')'                    {$$ = newSyntaxNode (AST_PARENTHESES,0,$2,0,0,0,getLineNumber());}
    ;

arg_list: expr rest_arg_list           {$$ = newSyntaxNode (AST_ARG_LIST,0,$1,$2,0,0,getLineNumber());}
        |                              {$$ = 0;}
        ;

rest_arg_list: ',' expr rest_arg_list  {$$ = newSyntaxNode (AST_ARG_LIST,0,$2,$3,0,0,getLineNumber());}
             |                         {$$ = 0;}
             ;

type: KW_INT              {$$ = newSyntaxNode (AST_TINT,0,0,0,0,0,getLineNumber());}
    | KW_FLOAT            {$$ = newSyntaxNode (AST_TFLOAT,0,0,0,0,0,getLineNumber());}
    | KW_BYTE             {$$ = newSyntaxNode (AST_TBYTE,0,0,0,0,0,getLineNumber());}
    ;

value: LIT_INTEGER        {$$ = newSyntaxNode (AST_INTEGER,$1,0,0,0,0,getLineNumber());}
     | LIT_FLOAT          {$$ = newSyntaxNode (AST_FLOAT,$1,0,0,0,0,getLineNumber());}
     | LIT_CHAR           {$$ = newSyntaxNode (AST_CHAR,$1,0,0,0,0,getLineNumber());}
     | LIT_STRING         {$$ = newSyntaxNode (AST_STRING,$1,0,0,0,0,getLineNumber());}
     ;


%%

void yyerror (char *s){
  fprintf(stderr, "Line %d: %s\n", getLineNumber(), s);
  exit(3);
}
