%token_type {Identifier}
%default_type {Identifier}

%extra_argument {Compiler compiler}
/*%token_destructor  {  Indentifier_Destroy($$); }*/

%include {
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "compiler.h"
#include "util.h"
}

/*
	Set each OPERATORator priority. The priority is in increasing order.
	So the Priority of OPERATOR_ADD is more than OPERATOR_CMP.
	Similarly the Priority of OPERATOR_AND is more than OPERATOR_SET.
	The OPERATOR_SET has the least priority
*/
%left  OPERATOR_EQU.
%left  OPERATOR_AND OPERATOR_OR.
%left  OPERATOR_CMP OPERATOR_NEQ.
%left  OPERATOR_GTH OPERATOR_GTE OPERATOR_LTH OPERATOR_LTE.
%left  OPERATOR_ADD OPERATOR_SUB.
%left  OPERATOR_DIV OPERATOR_MUL OPERATOR_MOD.
%right OPERATOR_NOT.

/*
	On error print the following message and the set the global error flag
   	so we can stop the tokenzier from generating more tokens
*/
%syntax_error {
   compiler->error_flag = 1;
   fprintf(stderr, "Syntax error on line %d !!!\n", compiler->line_number);
}

all ::= stmt.

stmt ::= .
stmt ::= stmt simple_stmt.

%type expr {Identifier}
%type stmt_end {int}
%type OPERATOR_SEMI_COLON {int}

%destructor expr {Identifier_Destroy($$);}

stmt_end	::= OPERATOR_SEMI_COLON.					{Command(compiler, OPERATOR_SEMI_COLON);}
simple_stmt ::= expr(A) stmt_end.						{Identifier_Destroy(A);}

expr(A) ::= TOKEN_TYPE_IDENTIFIER(B).					{A = Identifier_Clone(B); Identifier_Destroy(B);}
expr(A) ::= TOKEN_TYPE_VARIABLE(B).						{A = Identifier_Clone(B); Identifier_Destroy(B);}

expr(A) ::= expr(B) OPERATOR_SUB   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_SUB, C);	}
expr(A) ::= expr(B) OPERATOR_ADD   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_ADD, C);	}
expr(A) ::= expr(B) OPERATOR_MUL   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_MUL, C);	}
expr(A) ::= expr(B) OPERATOR_DIV   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_DIV, C);	}
expr(A) ::= expr(B) OPERATOR_MOD   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_MOD, C);	}
expr(A) ::= expr(B) OPERATOR_GTH   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_GTH, C);	}
expr(A) ::= expr(B) OPERATOR_LTH   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_LTH, C);	}
expr(A) ::= expr(B) OPERATOR_GTE   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_GTE, C);	}
expr(A) ::= expr(B) OPERATOR_LTE   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_LTE, C);	}
expr(A) ::= expr(B) OPERATOR_CMP   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_CMP, C);	}
expr(A) ::= expr(B) OPERATOR_NEQ   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_NEQ, C);	}
expr(A) ::= expr(B) OPERATOR_OR    expr(C).				{A = Command_Operation(compiler, B, OPERATOR_OR,  C);	}
expr(A) ::= expr(B) OPERATOR_AND   expr(C).				{A = Command_Operation(compiler, B, OPERATOR_AND, C);	}

expr(A) ::= OPERATOR_OPEN_PAREN  expr(B) OPERATOR_CLOSE_PAREN.


expr(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_EQU   expr(C).	{ A = Command_Operation(compiler, B, OPERATOR_EQU, C); }

