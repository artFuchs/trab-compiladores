%{
  #include "tokens.h"
  #include "hash.h"
  int running = 1;
  int lineNumber = 1;
%}
%x COMMENT

%%
\n                                    { printf ("line = %d\n", lineNumber++);}

ZZ.*
XXX                                   {BEGIN(COMMENT);}
<COMMENT>\n                           {printf ("line = %d\n", lineNumber++);}
<COMMENT>XXX                          {BEGIN(0);}
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
"&&"                                  { return OPERATOR_AND; }
"||"                                  { return OPERATOR_OR; }
"not"                                 { return OPERATOR_NOT; }
[a-z0-9_]+                            { hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER; }
[OIHGFEDCBA]+                         { hashInsert(LIT_INTEGER, yytext); return LIT_INTEGER; }
[OIHGFEDCBA]+\.[OIHGFEDCBA]+           { hashInsert(LIT_FLOAT, yytext); return LIT_FLOAT; }
\'.\'                                 { hashInsert(LIT_CHAR, yytext); return LIT_CHAR; }
\".*\"                      { hashInsert(LIT_STRING, yytext); return LIT_STRING; }
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

int getLineNumber(){
  return lineNumber;
}
