#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "compiler.h"
#include "tokenizer.h"
#include "util.h"

#include "grammer.h"



unsigned int line_number;


Identifier Command(Compiler c, Identifier A, int oper, Identifier B)
{
	static int reg_num = 0;
	char buf1[100], buf2[100], buf3[100];
	Identifier res = Identifier_NewRegister(reg_num++);
	Identifier_to_str(A, buf1, 100);
	Identifier_to_str(B, buf2, 100);
	Identifier_to_str(res, buf3, 100);
	LOG_INFO_NL("%s = %s %s %s", buf3, buf1, token_to_str(oper), buf2);
	return res;
}

int main(int argc, char *argv[])
{
	int yv;
	void *pParser;
	Compiler c;

	if(argc == 1)
	{
		LOG_INFO_NL("usage %s filename", argv[0]);
		return 1;
	}

	yyin = fopen(argv[1], "r");
	if(NULL == yyin)
	{
		printf("unable to open file\n");
		return 1;
	}

	pParser = (void*)ParseAlloc(Malloc);
	while((yv=yylex()) != 0)
	{
		c.line_number = line_number;
		Parse(pParser, yv, yylval, c);
		if(c.error_flag == 1)
		{
			fprintf(stderr, "stopping parsing due to syntax error\n");
			break;
		}
	}
	Parse(pParser, 0, yylval, c);
	ParseFree(pParser, Free);
	fclose(yyin);
	yylex_destroy();
	return 0;
}
