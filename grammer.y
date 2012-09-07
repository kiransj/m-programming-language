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

/*
	The program consists of statments. 
	The rule "all" is the master rule. Every character of program should be
	compatible with the rules laid down by all. Any mismatch is created as
	compilation error.
*/
all ::= stmt.

/*
	Statments can consists of the following.
	Stmt can be simple stmt (that is an expression ended with semi colon)
	Stmt can be condition stmt that is a if..endif block.
	stmt can have loops.
	Stmt can also have combinations of these by this recursive rule

   The below grammer rules allows us to embed simple_stmt, condition_stmt and loop_stmt
   in any order. That is this rule allows to use simple_stmt before and after condition_stmt
   with loop_stmt appearing anywhere

	Example if
	a = simple_stmt
	b = condition_stmt
	c = loop_stmt.
	x = stmt_list
	the grammer is defined as

	x -> .  				#there is no statments
	x -> x a 				#stmt_list can end with simple_stmt
	x -> x c				#stmt_list can end with loop_stmt
	x -> x b				#stmt_list can end with condition_stat

	Now any combination of [abc]* can be deduced using the above rules

	Example  to get cbca

	1> x -> x a
	2> x -> x c
	3> x -> x b
	4> x -> x c
	5> x -> .

	The above 5 rules when applied to together can deduce "cbca".
	Similarly any combination of stmts can be produced using the below rules
*/
stmt ::= .
stmt ::= stmt simple_stmt.
stmt ::= stmt condition_stmt.
stmt ::= stmt loop_stmt.
stmt ::= stmt variable_declaration.

/*
	STMT end operator is defined by SEMI COLON
*/
stmt_end	::= OPERATOR_SEMI_COLON.					{Command(compiler, STMT_END);}

/*
	A simple_stmt is an expression ended by stmt_end operator. 
*/
simple_stmt ::= expr(A) stmt_end.						{Identifier_Destroy(A);}

/*
	Expression can consists of the following.
	1. expression of mathamatical operators. (like +, -, / etc)
	2. Expression can be an assignment to variables or const. (like a=2)
	3. Expression can be act of calling functions.
	4. A combination of above 3 functions.
*/

expr(A) ::= TOKEN_TYPE_IDENTIFIER(B).					{A = Identifier_Clone(B); Identifier_Destroy(B);}
expr(A) ::= TOKEN_TYPE_VARIABLE(B).						{A = Identifier_Clone(B); Identifier_Destroy(B);}
expr(A) ::= function_call(B).							{A = Identifier_Clone(B); Identifier_Destroy(B);}

/*
	The defination of mathamatical operations supported
	by this language. These operations can be performed on
	variables, constants or return value of functions
*/
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

/*
	Handle parenthisis in the expressions.
*/
expr(A) ::= OPERATOR_OPEN_PAREN  expr(B) OPERATOR_CLOSE_PAREN.

/*
	The assignment operations has restrictions.
	The syntax for assignment operator is

	Variable := expr 

	This rules explicitily avoids operations like

	1=2; invalid
	1=a; invliad
*/

expr(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_EQU   expr(C).	{ A = Command_Operation(compiler, B, EQU, C); }

/*
	Function call grammer.
	The syntax for function call.

	Function_name(argument_list)

	Function_name has the same rule as that of variable name.
	argument_list can be empty
	argument_list can have on argument
	argument_list can have more than one argument with each one seperating other by a COMMA.

	The number of arguments passed to the function are counted.
*/

%type argument_list {int}

function_call(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_OPEN_PAREN  argument_list(C)	OPERATOR_CLOSE_PAREN.			{A = Command_function_call(compiler, B, C);}

argument_list(A) ::= .                              				{ A=0;} 
argument_list(A) ::= expr(B).                              			{ A=1; 	 Command_FunctionArg(compiler, B);} 
argument_list(A) ::= argument_list(B) OPERATOR_COMMA expr(C).       { A=B+1; Command_FunctionArg(compiler, C);}

/*
	if condition grammer.

	the syntax of if condition is

	if(exression)
		stmts;
	endif

	'if' and 'endif' are keywords.
	The stmts are executed if the expr returns a non zero value.
*/

%type start_if_condition {int}

end_if_condition 	 ::= KEYWORD_ENDIF.													 
start_if_condition(A)::= KEYWORD_IF OPERATOR_OPEN_PAREN expr(B) OPERATOR_CLOSE_PAREN.	{A=Command_ConditionStmt(compiler, STMT_IF, B, 0);}
if_condition_block	 ::= start_if_condition(A) stmt end_if_condition. 					{Command_ConditionStmt(compiler, STMT_ENDIF, NULL, A);}

condition_stmt ::= if_condition_block.

/*
	while loop grammer.

	the syntax of while loop is

	while(exression)
		stmts;
	endwhile

	'while' and 'endwhile' are keywords.
	The stmts are executed if the expr returns a non zero value.
*/

%type start_while_loop			{int}
%type while_condition_block		{int}

start_while_loop(A)			::= KEYWORD_WHILE.															{A=Command_LoopStmt(compiler, STMT_START_WHILE, NULL, 0);}
end_while_loop				::= KEYWORD_ENDWHILE.
while_condition_block(A)	::= start_while_loop(B) OPERATOR_OPEN_PAREN expr(C) OPERATOR_CLOSE_PAREN. 	{A=B;Command_LoopStmt(compiler, STMT_WHILE_COND, C, B);} 
while_loop_block			::= while_condition_block(B) stmt end_while_loop.							{Command_LoopStmt(compiler, STMT_END_WHILE, NULL, B);}

loop_stmt ::= while_loop_block.

/*
	Variable Declation list
*/
variable_declaration ::= KEYWORD_VAR variable_list stmt_end.           
variable_list ::= TOKEN_TYPE_VARIABLE(B).                               { Command_VariableDecl(compiler, B);} 
variable_list ::= variable_list OPERATOR_COMMA  TOKEN_TYPE_VARIABLE(B). { Command_VariableDecl(compiler, B);}
