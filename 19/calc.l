%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "calc.tab.h"

%}

%%

[ \t]	; // ignore all whitespace
\n		{return T_NEWLINE;}
"a"		{return T_A;}
"b"		{return T_B;}

%%
