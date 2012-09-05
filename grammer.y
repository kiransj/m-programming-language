%token_type {Token}
%default_type {Token}
%token_destructor  { TokenDestroy($$); }

%include {
#include <stdio.h>
#include <stdlib.h>
#include "eval.h"

#include "tokenizer.h"

}

%extra_argument {Executable exe}
/*
	Set each operator priority. The priority is in increasing order.
	So the Priority of OPER_ADD is more than OPER_CMP.
	Similarly the Priority of OPER_AND is more than OPER_SET.
	The OPER_SET has the least priority
*/
%left  OPER_SET OPER_ADDEQ OPER_SUBEQ.
%left  OPER_AND OPER_OR.
%left  OPER_CMP OPER_NEQ.
%left  OPER_GTH OPER_GTE OPER_LTH OPER_LTE.
%left  OPER_ADD OPER_SUB.
%left  OPER_DIV OPER_MUL OPER_MOD.
%right OPER_POW OPER_NOT.

/*
	On error print the following message and the set the global error flag
   	so we can stop the tokenzier from generating more tokens
*/
%syntax_error {
   exe->error_flag = 1;
   fprintf(stderr, "Syntax error on line %d !!!\n", exe->line_number);
}


