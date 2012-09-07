GCC=gcc
CFLAGS := -g 
WARNINGS:= -Wall
LEMON:=lemon

GRAMMER_FILE:=grammer.y
LEXER_FILE:=lexer.l

SOURCE := compiler.c
SOURCE += util.c
SOURCE += tokenizer.c
SOURCE += executable.c
SOURCE += execute_context.c
SOURCE += function.c
SOURCE += native_functions.c

COMPILER_OBJECTS :=
COMPILER_OBJECTS += $(GRAMMER_FILE:.y=.o)
COMPILER_OBJECTS += $(LEXER_FILE:.l=.o)
COMPILER_OBJECTS += $(SOURCE:.c=.o)

INCLUDES:= -I.

OUTPUT:=a.out

all: $(OUTPUT)

$(OUTPUT): $(COMPILER_OBJECTS)
	@echo "building $(OUTPUT)"
	@$(GCC) -o $(OUTPUT) $(CFLAGS) $(INCLUDES) $(COMPILER_OBJECTS)

%.o:%.l
	@echo "compiling $^"
	@flex --outfile=`basename $^ .l`.c $(LEXER_FILE)
	@$(GCC) $(CFLAGS) -c `basename $^ .l`.c -o $@
	@rm `basename $^ .l`.c

%.o:%.y
	@echo "compiling $^"
	@$(LEMON) -q $^
	@$(GCC) -g $(INCLUDES) -c `basename $^ .y`.c -o $@
	@rm `basename $^ .y`.c

%.o:%.c
	@echo "compiling $^"
	@$(GCC) $(CFLAGS) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(COMPILER_OBJECTS) $(OUTPUT) *core*

