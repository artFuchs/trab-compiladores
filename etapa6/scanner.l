%{
  /******************************************************
  *                                                     *
  * Autores: Arthur Lucena Fuchs e Matheus Westhelle    *
  *                                                     *
  ******************************************************/
  #include "y.tab.h"
  #include "hash.h"
  #include "ast.h"
  #include "symbols.h"
  #include "types.h"

  extern YYSTYPE yylval;

  int running;
  int lineNumber;
  char buf[1024];
  char *s;


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
[a-z0-9_]+                            { yylval.symbol = hashInsert(SYMBOL_IDENTIFIER, DATATYPE_UNDEFINED, yytext); return TK_IDENTIFIER; }
[OIHGFEDCBA]+                         { yylval.symbol = hashInsert(SYMBOL_LIT_INT, DATATYPE_INT, yytext); return LIT_INTEGER; }
[OIHGFEDCBA]+\.[OIHGFEDCBA]+          { yylval.symbol = hashInsert(SYMBOL_LIT_FLOAT, DATATYPE_FLOAT, yytext); return LIT_FLOAT; }
\'.\'                                 { yylval.symbol = hashInsert(SYMBOL_LIT_CHAR, DATATYPE_CHAR, yytext); return LIT_CHAR; }
\"                                    { BEGIN(STRING); s = buf; }
[ \t]
.                                     { return TOKEN_ERROR; }

<STRING>\\n                           { *s++ = '\\'; *s++ = 'n'; }
<STRING>\\t                           { *s++ = '\\'; *s++ = 't'; }
<STRING>\\\"                          { *s++ = '\\'; *s++ = '\"'; }
<STRING>\"                            {
                                        *s = 0;
                                        BEGIN 0;
                                        yylval.symbol = hashInsert(SYMBOL_LIT_STRING, DATATYPE_STRING, buf);
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
  functions_asts = NULL;
  functions_asts_size = 0;
}
