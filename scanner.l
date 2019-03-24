%{
  #include "tokens.h"
  #include "hash.h"
  int running = 1;
  int lineNumber = 1;
  char buf[1024];
  char *s;
%}

%x STRING

%%


\n                                    { printf ("line = %d\n", lineNumber++);}
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
[a-z0-9_]+                            { hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER; }
[OIHGFEDCBA]+                         { hashInsert(LIT_INTEGER, yytext); return LIT_INTEGER; }
[OIHGFEDCBA]+.[OIHGFEDCBA]+           { hashInsert(LIT_FLOAT, yytext); return LIT_FLOAT; }
\'.\'                                 { hashInsert(LIT_CHAR, yytext); return LIT_CHAR; }
\"                                    { BEGIN(STRING); s = buf; }
[ \t]
.                                     { return TOKEN_ERROR; }

<STRING>\\n                           { *s++ = '\n'; }
<STRING>\\t                           { *s++ = '\t'; }
<STRING>\\\"                          { *s++ = '\"'; }
<STRING>\"                            {
                                        *s = 0;
                                        BEGIN 0;
                                        hashInsert(LIT_STRING, buf);
                                        return LIT_STRING;
                                      }
<STRING>\n                            { printf("string invalida"); exit(1); }
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
