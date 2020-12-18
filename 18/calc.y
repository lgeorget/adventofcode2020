%{

#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}

%union{
	long long int ival;
};

%token<ival> T_INT
%token T_NEWLINE
%token T_LEFT T_RIGHT
// for part 2, this inverts the usual precedence of * and +
%left T_MULTIPLY
%left T_PLUS
// for part 1
// %left T_MULTIPLY T_PLUS

%type<ival> expression
%type<ival> calculation

%start total

%%

total: calculation { printf("Result: %li\n", $1); }
;

calculation: expression T_NEWLINE calculation { $$ = $1 + $3; }
	   | /* empty */ { $$ = 0; }
;

expression: T_INT				{ $$ = $1; }
	  | expression T_PLUS expression	{ $$ = $1 + $3; }
	  | expression T_MULTIPLY expression	{ $$ = $1 * $3; }
	  | T_LEFT expression T_RIGHT		{ $$ = $2; }
;

%%

int main() {
	yyin = stdin;

	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
