%{
  #include "tokens.h"
  int running = 1;
%}

%%
\n                                    { printf ("line = %d\n", yylineno++);}
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
"&&"                                  { return OPERATOR_AND; }
"||"                                  { return OPERATOR_OR; }
"not"                                 { return OPERATOR_NOT; }
[a-z0-9_]+                            { return TK_IDENTIFIER; }
[OIHGFEDCBA]+                         { return LIT_INTEGER; }
[OIHGFEDCBA]+.[OIHGFEDCBA]+           { return LIT_FLOAT; }
\'.\'                                 { return LIT_CHAR; }
\"[a-zA-Z0-9]*\"                      { return LIT_STRING; }
[ \t]
.                                     { return TOKEN_ERROR; }
%%

int yywrap() {
  running = 0;
  return 1;
}

int isRunning() {
  return running;
}
