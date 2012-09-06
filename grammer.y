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
stmt ::= stmt condition_stmt.
stmt ::= stmt loop_stmt.

stmt_end	::= OPERATOR_SEMI_COLON.					{Command(compiler, STMT_END);}
simple_stmt ::= expr(A) stmt_end.						{Identifier_Destroy(A);}


expr(A) ::= TOKEN_TYPE_IDENTIFIER(B).					{A = Identifier_Clone(B); Identifier_Destroy(B);}
expr(A) ::= TOKEN_TYPE_VARIABLE(B).						{A = Identifier_Clone(B); Identifier_Destroy(B);}
expr(A) ::= function_call(B).							{A = Identifier_Clone(B); Identifier_Destroy(B);}

expr(A) ::= expr(B) OPERATOR_SUB   expr(C).				{A = Command_Operation(compiler, B, SUB, C);	}
expr(A) ::= expr(B) OPERATOR_ADD   expr(C).				{A = Command_Operation(compiler, B, ADD, C);	}
expr(A) ::= expr(B) OPERATOR_MUL   expr(C).				{A = Command_Operation(compiler, B, MUL, C);	}
expr(A) ::= expr(B) OPERATOR_DIV   expr(C).				{A = Command_Operation(compiler, B, DIV, C);	}
expr(A) ::= expr(B) OPERATOR_MOD   expr(C).				{A = Command_Operation(compiler, B, MOD, C);	}
expr(A) ::= expr(B) OPERATOR_GTH   expr(C).				{A = Command_Operation(compiler, B, GTH, C);	}
expr(A) ::= expr(B) OPERATOR_LTH   expr(C).				{A = Command_Operation(compiler, B, LTH, C);	}
expr(A) ::= expr(B) OPERATOR_GTE   expr(C).				{A = Command_Operation(compiler, B, GTE, C);	}
expr(A) ::= expr(B) OPERATOR_LTE   expr(C).				{A = Command_Operation(compiler, B, LTE, C);	}
expr(A) ::= expr(B) OPERATOR_CMP   expr(C).				{A = Command_Operation(compiler, B, CMP, C);	}
expr(A) ::= expr(B) OPERATOR_NEQ   expr(C).				{A = Command_Operation(compiler, B, NEQ, C);	}
expr(A) ::= expr(B) OPERATOR_OR    expr(C).				{A = Command_Operation(compiler, B, OR,  C);	}
expr(A) ::= expr(B) OPERATOR_AND   expr(C).				{A = Command_Operation(compiler, B, AND, C);	}

expr(A) ::= OPERATOR_OPEN_PAREN  expr(B) OPERATOR_CLOSE_PAREN.

expr(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_EQU   expr(C).	{ A = Command_Operation(compiler, B, EQU, C); }


%type argument_list {int}

function_call(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_OPEN_PAREN  argument_list(C)	OPERATOR_CLOSE_PAREN.			{A = Command_function_call(compiler, B, C);}

argument_list(A) ::= .                              				{ A=0;} 
argument_list(A) ::= expr(B).                              			{ A=1; 	 Command_FunctionArg(compiler, B, A);} 
argument_list(A) ::= argument_list(B) OPERATOR_COMMA expr(C).       { A=B+1; Command_FunctionArg(compiler, C, A);}

%type start_if_condition {int}

end_if_condition 	 ::= KEYWORD_ENDIF.													 
start_if_condition(A)::= KEYWORD_IF OPERATOR_OPEN_PAREN expr(B) OPERATOR_CLOSE_PAREN.	{A=Command_ConditionStmt(compiler, STMT_IF, B, 0);}
if_condition_block	 ::= start_if_condition(A) stmt end_if_condition. 					{Command_ConditionStmt(compiler, STMT_ENDIF, NULL, A);}

condition_stmt ::= if_condition_block.

%type start_while_loop			{int}
%type while_condition_block		{int}

start_while_loop(A)			::= KEYWORD_WHILE.															{A=Command_LoopStmt(compiler, STMT_START_WHILE, NULL, 0);}
end_while_loop				::= KEYWORD_ENDWHILE.
while_condition_block(A)	::= start_while_loop(B) OPERATOR_OPEN_PAREN expr(C) OPERATOR_CLOSE_PAREN. 	{A=B;Command_LoopStmt(compiler, STMT_WHILE_COND, C, B);} 
while_loop_block			::= while_condition_block(B) stmt end_while_loop.							{Command_LoopStmt(compiler, STMT_END_WHILE, NULL, B);}

loop_stmt ::= while_loop_block.
