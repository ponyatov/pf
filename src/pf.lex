%{
    #include "pf.hpp"
    char *yyfile;
%}

%option noyywrap yylineno

S [+\-]?
N [0-9]+

%%
#.*             {}                                              // line comment
[ \t\r\n]+      {}                                              // drop spaces

"{"             { return LC; }
"}"             { return RC; }
":"             { return COLON; }

"nop"           { yylval.c = op::nop;  return CMD0; }
"halt"          { yylval.c = op::halt; return CMD0; }

".entry"        { yylval.s = new string(yytext); return ENTRY; }

{S}{N}          { yylval.n = atoi(yytext); return INT; }        // number
[^ \t\r\n]+     { yylval.s = new string(yytext); return SYM; }  // symbol

.               { yyerror("lexer"); }                           // any undetected char
