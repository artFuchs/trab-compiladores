/******************************************************
*                                                     *
* Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
*                                                     *
******************************************************/

#ifndef AST_CONST
#define AST_CONST

#define MAX_SONS 4

#define AST_SYMBOL 1

#define AST_ADD 10
#define AST_SUB 11
#define AST_MUL 12
#define AST_DIV 13

#define AST_AND 20
#define AST_OR 21
#define AST_NOT 22
#define AST_LT 23
#define AST_GT 24
#define AST_LE 25
#define AST_GE 26
#define AST_EQ 27

#define AST_ASSIGN 30
#define AST_ARRAY_ASSIGN 31

#define AST_IF 40
#define AST_IF_ELSE 41
#define AST_LOOP 42
#define AST_LEAP

#define AST_NO_DELIMITER_LIST 50
#define AST_COMMA_DELIMITED_LIST 51
#define AST_SEMICOLON_DELIMITED_LIST 52

#define AST_PRINT 60
#define AST_READ 61
#define AST_RETURN 62

#define AST_CMD_BLOCK 70
#define AST_PARENTHESES 71

#define AST_INTEGER 100
#define AST_FLOAT 101
#define AST_CHAR 102
#define AST_STRING 103

#endif
