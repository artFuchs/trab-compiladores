%{
  /******************************************************
  *                                                     *
  * Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
  *                                                     *
  ******************************************************/
  #include "y.tab.h"
  #include "hash.h"

  extern YYSTYPE yylval;

  int running;
  int lineNumber;
  char buf[1024];
  char *s;

  #define SYMBOL_LIT_INT 1
  #define SYMBOL_LIT_FLOAT 2
  #define SYMBOL_LIT_BYTE 3
  #define SYMBOL_LIT_CHAR 4
  #define SYMBOL_LIT_STRING 5
  #define SYMBOL_IDENTIFIER 6

%}
%x COMMENT
%x STRING

%%


\n                                    { lineNumber++; }

ZZ.*
XXX                                   { BEGIN(COMMENT);}
<COMMENT>\n                           { lineNumber++; }
<COMMENT>XXX                          { BEGIN(0);}
<COMMENT>.*

"byte"                                { return KW_BYTE; }
"int"                                 { return KW_INT; }
"float"                               { return KW_FLOAT; }
"if"                                  { return KW_IF; }
"then"                                { return KW_THEN; }
"else"                                { return KW_ELSE; }
"loop"                                { return KW_LOOP; }
"leap"                                { return KW_LEAP; }
"read"                                { return KW_READ; }
"return"                              { return KW_RETURN; }
"print"                               { return KW_PRINT; }
[-+*/<>=&$#,;:()\[\]{}]               { return (int) yytext[0]; }
"<="                                  { return OPERATOR_LE; }
">="                                  { return OPERATOR_GE; }
"=="                                  { return OPERATOR_EQ; }
"!="                                  { return OPERATOR_DIF; }
"and"                                 { return OPERATOR_AND; }
"or"                                  { return OPERATOR_OR; }
"not"                                 { return OPERATOR_NOT; }
[a-z0-9_]+                            { yylval.symbol = hashInsert(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }
[OIHGFEDCBA]+                         { yylval.symbol = hashInsert(SYMBOL_LIT_INT, yytext); return LIT_INTEGER; }
[OIHGFEDCBA]+\.[OIHGFEDCBA]+          { yylval.symbol = hashInsert(SYMBOL_LIT_FLOAT, yytext); return LIT_FLOAT; }
\'.\'                                 { yylval.symbol = hashInsert(SYMBOL_LIT_CHAR, yytext); return LIT_CHAR; }
\"                                    { BEGIN(STRING); s = buf; }
[ \t]
.                                     { return TOKEN_ERROR; }

<STRING>\\n                           { *s++ = '\n'; }
<STRING>\\t                           { *s++ = '\t'; }
<STRING>\\\"                          { *s++ = '\"'; }
<STRING>\"                            {
                                        *s = 0;
                                        BEGIN 0;
                                        yylval.symbol = hashInsert(SYMBOL_LIT_STRING, buf);
                                        return LIT_STRING;
                                      }
<STRING>\n                            { printf("string invalida"); lineNumber++; return TOKEN_ERROR; }
<STRING>.                             { *s++ = *yytext; }

%%

int yywrap() {
  running = 0;
  return 1;
}

int isRunning() {
  return running;
}

int getLineNumber(){
  return lineNumber;
}

void initMe(){
  lineNumber = 1;
  running = 1;
  hashInit();
}
