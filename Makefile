GCC=gcc
CFLAGS = -g

GRAMMER_FILE:=eval.y
LEXER_FILE:=tokenizer.l

SOURCE := compile.c 
COMPILER_OBJECTS := $(SOURCE:.c=.o)
COMPILER_OBJECTS += $(GRAMMER_FILE:.y=.o)
COMPILER_OBJECTS += $(LEXER_FILE:.l=.o)

INCLUDES:= -I. -I../

all: $(LIBRARY_NAME)


%.o:%.l
	@echo "compiling $^"
	@flex --outfile=`basename $^ .l`.c tokenizer.l
	@$(GCC) $(CFLAGS) -c `basename $^ .l`.c -o $@
	@rm `basename $^ .l`.c

%.o:%.y
	@echo "compiling $^"
	@./lemon.out -q $^
	@gcc $(CFLAGS) $(INCLUDES) -c `basename $^ .y`.c -o $@
	@rm `basename $^ .y`.c

%.o:%.c
	@echo "compiling $^"
	@$(GCC) $(CFLAGS) $(INCLUDES) -c $^  -o $@
	
clean:
	@rm -f $(LEMON_OUTPUT) $(COMPILER_OBJECTS) $(LIBRARY_NAME) 
