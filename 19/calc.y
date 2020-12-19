%{

#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}

%glr-parser

%union{
	long long int ival;
};

%token T_A T_B
%token T_NEWLINE

%type<ival> total

%start total

%%

total: e0 T_NEWLINE { printf("Result: %li\n", 1); }
;

e25: e53 e116;
e122: e116 e92 | e53 e53;
e100: e53 e73 | e116 e125;
e111: e67 e116 | e91 e53;
e71: e116 e58 | e53 e78;
e55: e116 e54 | e53 e21;
e123: e53 e97 | e116 e108;
e104: e56 e116 | e125 e53;
e19: e125 e53;
e56: e116 e116 | e116 e53;
e60: e116 e121 | e53 e59;
e75: e53 e20 | e116 e124;
e6: e53 e131 | e116 e33;
e62: e53 e53 | e116 e116;
e101: e116 e127 | e53 e26;
e128: e116 e125 | e53 e93;
e34: e53 e28 | e116 e125;
e65: e63 e116 | e135 e53;
e50: e32 e53 | e76 e116;
e2: e53 e10 | e116 e132;
e133: e28 e53 | e131 e116;
e85: e53 e131;
e125: e116 e53;
e118: e87 e116 | e3 e53;
e135: e73 e53 | e125 e116;
e28: e53 e53;
e110: e116 e122 | e53 e56;
e91: e5 e53 | e74 e116;
e73: e92 e116 | e53 e53;
e80: e15 e116 | e109 e53;
e124: e128 e116 | e17 e53;
e87: e92 e53 | e53 e116;
e43: e116 e27 | e53 e37;
e66: e46 e53 | e93 e116;
e106: e116 e77 | e53 e83;
e137: e72 e116 | e104 e53;
e93: e116 e116 | e53 e116;
e129: e92 e103;
e12: e116 e56 | e53 e131;
e45: e53 e132 | e116 e12;
e102: e65 e116 | e2 e53;
e24: e116 e101 | e53 e95;
e14: e116 e84 | e53 e29;
e57: e93 e116 | e56 e53;
e70: e108 e116 | e100 e53;
e51: e116 e1 | e53 e68;
e26: e73 e53;
e15: e116 e93 | e53 e125;
e42: e53 e134 | e116 e106;
e79: e116 e33 | e53 e122;
e5: e116 e125 | e53 e3;
e74: e53 e122 | e116 e73;
e18: e93 e53 | e87 e116;
e20: e47 e53 | e17 e116;
e114: e116 e85 | e53 e118;
e37: e53 e73 | e116 e87;
e47: e116 e73 | e53 e125;
e107: e116 e3 | e53 e46;
e69: e53 e25 | e116 e125;
e32: e53 e28 | e116 e93;
e76: e3 e53 | e125 e116;
e38: e92 e33;
e119: e116 e39 | e53 e80;
e8: e42 | e42 e8;
e21: e125 e53 | e125 e116;
e58: e75 e116 | e119 e53;
e64: e116 e44 | e53 e113;
e68: e90 e53 | e45 e116;
e1: e23 e116 | e60 e53;
e53: T_A;
e81: e73 e53 | e33 e116;
e86: e116 e129 | e53 e35;
e90: e110 e116 | e13 e53;
e82: e53 e93 | e116 e25;
e95: e99 e116 | e66 e53;
e27: e116 e56 | e53 e3;
e120: e116 e51 | e53 e40;
e126: e116 e96 | e53 e52;
e116: T_B;
e84: e116 e123 | e53 e49;
e134: e116 e61 | e53 e14;
e78: e116 e102 | e53 e86;
e96: e130 e53 | e98 e116;
e10: e87 e116 | e33 e53;
e88: e53 e116 | e116 e92;
e40: e126 e53 | e24 e116;
e127: e56 e116;
e59: e87 e53 | e3 e116;
e11: e42 e31 | e42 e11 e31;
e41: e116 e137 | e53 e16;
e44: e116 e22 | e53 e79;
e109: e88 e53 | e87 e116;
e49: e136 e116 | e94 e53;
e113: e107 e116 | e81 e53;
e77: e116 e64 | e53 e36;
e3: e53 e116 | e53 e53;
e9: e133 e53 | e112 e116;
e98: e92 e93;
e117: e3 e92;
e39: e53 e19 | e116 e21;
e16: e6 e116 | e117 e53;
e132: e131 e116 | e125 e53;
e63: e116 e122 | e53 e33;
e22: e131 e116 | e56 e53;
e89: e116 e125 | e53 e131;
e115: e55 e53 | e50 e116;
e99: e116 e33 | e53 e93;
e92: e116 | e53;
e121: e53 e73 | e116 e56;
e13: e73 e92;
e103: e53 e3 | e116 e62;
e83: e111 e116 | e41 e53;
e61: e105 e116 | e115 e53;
e130: e56 e92;
e136: e33 e116 | e125 e53;
e30: e53 e76 | e116 e7;
e94: e92 e87;
e52: e97 e116 | e18 e53;
e7: e116 e28 | e53 e46;
e97: e25 e53 | e33 e116;
e35: e89 e116 | e82 e53;
e72: e53 e131 | e116 e87;
e48: e34 e116 | e38 e53;
e54: e116 e46 | e53 e131;
e131: e116 e116;
e23: e116 e4 | e53 e57;
e31: e116 e71 | e53 e120;
e29: e116 e30 | e53 e114;
e108: e116 e25 | e53 e46;
e33: e92 e92;
e4: e131 e116 | e3 e53;
e36: e70 e116 | e9 e53;
e67: e69 e116 | e117 e53;
e0: e8 e11;
e105: e48 e116 | e43 e53;
e46: e116 e53 | e53 e116;
e17: e93 e116 | e122 e53;
e112: e122 e53 | e3 e116;

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
	//exit(1);
}

