%token_type {Identifier}
%default_type {Identifier}

%extra_argument {Compiler compiler}

%include {
#include <stdio.h>
#include <stdlib.h>

#include "identifier.h"
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
all ::= program.

program ::= .
program ::= program    function_body.


function_decl_start ::= KEYWORD_FUNCTION TOKEN_TYPE_VARIABLE(A)  OPERATOR_OPEN_PAREN   OPERATOR_CLOSE_PAREN.  {Command_NewFunction(compiler, A);}
function_decl_end   ::= KEYWORD_ENDFUNCTION.                                                                  {Command_EndFunction(compiler);}
function_body       ::= function_decl_start stmt function_decl_end.

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

    x -> .                  #there is no statments
    x -> x a                 #stmt_list can end with simple_stmt
    x -> x c                #stmt_list can end with loop_stmt
    x -> x b                #stmt_list can end with condition_stat

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
stmt ::= stmt return_stmt.

/*
    STMT end operator is defined by SEMI COLON
*/
stmt_end    ::= OPERATOR_SEMI_COLON.                    {Command(compiler, STMT_END);}

/*
    A simple_stmt is an expression ended by stmt_end operator.
*/
simple_stmt ::= expr(A) stmt_end.                        {Identifier_Destroy(A);}

/*
    Expression can consists of the following.
    1. expression of mathamatical operators. (like +, -, / etc)
    2. Expression can be an assignment to variables or const. (like a=2)
    3. Expression can be act of calling functions.
    4. A combination of above 3 functions.
*/

/*
    Object refs Syntax.
    obj->variable_name.

    example Usage
    var file;
    file->name = "tmp.txt";
    file->size = 100;

    Recursive syntax is not allowed. That is

    file->tmp->a; Syntax error

*/

%type object_refs {Identifier}
object_refs(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_OBJECT_REF TOKEN_TYPE_VARIABLE(C).{
                                                                                        A=Identifier_NewMap(B->u.variable_name, C->u.variable_name);
                                                                                        Identifier_Destroy(B);
                                                                                        Identifier_Destroy(C);
                                                                                     }

/*
    Array feature implementation.
    a[10] = "temp";
    a["temp"] = 10;
*/
%type array_refs {Identifier}
array_refs(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_OPEN_SQUARE expr(C) OPERATOR_CLOSE_SQUARE.{
                                                                                               A=Identifier_NewArray(B->u.variable_name, C);
                                                                                               Identifier_Destroy(B);
                                                                                               Identifier_Destroy(C);
                                                                                           }
/*
    Primitive data type supported by M.
    Integers : 0, 1 etc
    Strings  : "string", "str" etc
    Arguments : $0 => number of arguments, $1, $2 etc...
    Variable  : a, b etc, which are decalared using the var keyword
*/

primitive_data_type(A) ::= TOKEN_TYPE_INTEGER(B).               {A = B;}
primitive_data_type(A) ::= TOKEN_TYPE_STRING(B).                {A = B;}
primitive_data_type(A) ::= TOKEN_TYPE_ARGUMENT(B).              {A = B;}
primitive_data_type(A) ::= TOKEN_TYPE_VARIABLE(B).              {A = B;}

/*
    Syntax for array.
    Array_name[primitive_data_type]. This syntax allows the following

    a[10], system["time"], system[a]
*/

expr(A) ::= primitive_data_type(B).                 {A = B;}
expr(A) ::= function_call(B).                       {A = B;}
expr(A) ::= object_refs(B).                         {A = B;}
expr(A) ::= array_refs(B).                          {A = B;}

/*Handle negative number*/
expr(A) ::= OPERATOR_SUB TOKEN_TYPE_INTEGER(B).        {B->u.number = -B->u.number; A = B;}
/*
    The defination of mathamatical operations supported
    by this language. These operations can be performed on
    variables, constants or return value of functions
*/
expr(A) ::= expr(B) OPERATOR_SUB   expr(C).                {A = Command_Operation(compiler, B, SUB, C);    }
expr(A) ::= expr(B) OPERATOR_ADD   expr(C).                {A = Command_Operation(compiler, B, ADD, C);    }
expr(A) ::= expr(B) OPERATOR_MUL   expr(C).                {A = Command_Operation(compiler, B, MUL, C);    }
expr(A) ::= expr(B) OPERATOR_DIV   expr(C).                {A = Command_Operation(compiler, B, DIV, C);    }
expr(A) ::= expr(B) OPERATOR_MOD   expr(C).                {A = Command_Operation(compiler, B, MOD, C);    }
expr(A) ::= expr(B) OPERATOR_GTH   expr(C).                {A = Command_Operation(compiler, B, GTH, C);    }
expr(A) ::= expr(B) OPERATOR_LTH   expr(C).                {A = Command_Operation(compiler, B, LTH, C);    }
expr(A) ::= expr(B) OPERATOR_GTE   expr(C).                {A = Command_Operation(compiler, B, GTE, C);    }
expr(A) ::= expr(B) OPERATOR_LTE   expr(C).                {A = Command_Operation(compiler, B, LTE, C);    }
expr(A) ::= expr(B) OPERATOR_CMP   expr(C).                {A = Command_Operation(compiler, B, CMP, C);    }
expr(A) ::= expr(B) OPERATOR_NEQ   expr(C).                {A = Command_Operation(compiler, B, NEQ, C);    }
expr(A) ::= expr(B) OPERATOR_OR    expr(C).                {A = Command_Operation(compiler, B, OR,  C);    }
expr(A) ::= expr(B) OPERATOR_AND   expr(C).                {A = Command_Operation(compiler, B, AND, C);    }
expr(A) ::= OPERATOR_NOT    expr(C).                       {A = Command_Operation(compiler, NULL, NOT, C); }

/*
    Handle parenthisis in the expressions.
*/
expr(A) ::= OPERATOR_OPEN_PAREN  expr(B) OPERATOR_CLOSE_PAREN.  {A = B;}

/*
    The assignment operations has restrictions.
    The syntax for assignment operator is

    Variable := expr

    This rules explicitily avoids operations like

    1=2; invalid
    1=a; invliad
*/

expr(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_EQU   expr(C).    { A = Command_Operation(compiler, B, EQU, C); }
expr(A) ::= object_refs(B)         OPERATOR_EQU   expr(C).    { A = Command_Operation(compiler, B, EQU, C); }
expr(A) ::= array_refs(B)          OPERATOR_EQU   expr(C).    { A = Command_Operation(compiler, B, EQU, C); }

/*
    Function call grammer.
    The syntax for function call.

    Function_name(argument_list)

    Function_name has the same rule as that of variable name.
    1. argument_list can be empty
    2. argument_list can have one argument
    3. argument_list can have more than one argument with each one seperating other by a COMMA.

    The number of arguments passed to the function are counted using the argument list variable
*/

%type argument_list {int}

function_call(A) ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_OPEN_PAREN  argument_list(C)    OPERATOR_CLOSE_PAREN.            {A = Command_function_call(compiler, B, C);}

argument_list(A) ::= .                                              { A=0; }
argument_list(A) ::= expr(B).                                       { A=1;  Command_FunctionArg(compiler, B);  }
argument_list(A) ::= argument_list(B) OPERATOR_COMMA expr(C).       { A=B+1; Command_FunctionArg(compiler, C); }

/*
    if condition grammer.

    the syntax of if condition is

    if(expression)
        stmts;
    endif

    'if' and 'endif' are keywords.
    The stmts are executed if the expression returns a non zero value.
*/

conditional_expression(A) ::= OPERATOR_OPEN_PAREN expr(B) OPERATOR_CLOSE_PAREN.      {A = B;}

elif_keyword         ::= KEYWORD_ELIF.                                               {Command_ConditionStmt(compiler, STMT_ELIF_KEYWORD, NULL);}
elif_condition       ::= elif_keyword conditional_expression(B).                     {Command_ConditionStmt(compiler, STMT_ELIF_CONDITION, B); }
else_condition       ::= KEYWORD_ELSE.                                               {Command_ConditionStmt(compiler, STMT_ELSE, NULL);        }
endif_condition      ::= KEYWORD_ENDIF.                                              {Command_ConditionStmt(compiler, STMT_ENDIF, NULL);       }
start_if_condition   ::= KEYWORD_IF conditional_expression(B).                       {Command_ConditionStmt(compiler, STMT_IF, B);             }

/*
if_condition_block     ::= start_if_condition stmt endif_condition.
if_condition_block     ::= start_if_condition stmt else_condition stmt endif_condition.
if_condition_block     ::= start_if_condition stmt elif_block endif_condition.

elif_block             ::= elif_condition stmt.
elif_block             ::= elif_block elif_condition stmt.
*/

if_condition_block   ::= start_if_condition stmt.
if_condition_block   ::= start_if_condition stmt elif_block.

elif_block       ::= elif_condition stmt.
elif_block       ::= elif_block elif_condition stmt.

condition_stmt   ::= if_condition_block endif_condition.
condition_stmt   ::= if_condition_block else_condition stmt endif_condition.

/*
    while loop grammer.

    the syntax of while loop is

    while(expression)
        stmts;
    endwhile

    'while' and 'endwhile' are keywords.
    The stmts are executed if the expression returns a non zero value.
*/

end_while_loop              ::= KEYWORD_ENDWHILE.
start_while_loop            ::= KEYWORD_WHILE.                                                           {Command_LoopStmt(compiler, STMT_START_WHILE, NULL);}
while_condition_block       ::= start_while_loop  conditional_expression(C).                             {Command_LoopStmt(compiler, STMT_WHILE_COND, C);}
while_loop_block            ::= while_condition_block stmt end_while_loop.                               {Command_LoopStmt(compiler, STMT_END_WHILE, NULL);}

loop_stmt ::= while_loop_block.

/*
    Variable Declation list. Variable DECL starts with the 'var' keyword
    and followed by the variable name.
    For more than one variale declaration the variable names should be seperated with a COMMA.
    Variable can be initialized on declaration
*/
variable_declaration ::= KEYWORD_VAR variable_list stmt_end.
variable_list        ::= variable_decl.
variable_list        ::= variable_list OPERATOR_COMMA  variable_decl.

/*
    Variable Declaration can be of 3 forms.

    1. var a, b;                     Here the variables are not initialized.
    2. var a := 10, b := a+10;        Here the variable are initialzed.
    4. var a, b:=10, c:=20;            Here some are initialzed and some are not.
*/

variable_decl ::= TOKEN_TYPE_VARIABLE(B).                               { Command_VariableDecl(compiler, B, NULL);}
variable_decl ::= TOKEN_TYPE_VARIABLE(B) OPERATOR_EQU expr(C) .         { Command_VariableDecl(compiler, B, C);}


/*
    Return STMT. Return stmt should have a return value.
    Syntax
    return (expr);
*/

return_stmt ::= KEYWORD_RETURN expr(A) OPERATOR_SEMI_COLON.              { Command_ReturnStmt(compiler, A);}
