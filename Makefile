GCC=gcc
CFLAGS := -g 
WARNINGS:= -Wall
LEMON:=lemon

GRAMMER_FILE:=src/grammer.y
LEXER_FILE:=src/lexer.l

SOURCE := src/compiler.c
SOURCE += src/util.c
SOURCE += src/tokenizer.c
SOURCE += src/executable.c
SOURCE += src/execute_context.c
SOURCE += src/function.c
SOURCE += src/native_functions.c

COMPILER_OBJECTS :=
COMPILER_OBJECTS += $(GRAMMER_FILE:.y=.o)
COMPILER_OBJECTS += $(LEXER_FILE:.l=.o)
COMPILER_OBJECTS += $(SOURCE:.c=.o)

INCLUDES:=  -Iinc -Isrc 

OUTPUT:=a.out

all: $(OUTPUT)

$(OUTPUT): $(COMPILER_OBJECTS)
	@echo "building $(OUTPUT)"
	@$(GCC) -o $(OUTPUT) $(CFLAGS) $(INCLUDES) $(COMPILER_OBJECTS)

%.o:%.l
	@echo "compiling $^"
	@flex --outfile=`basename $^ .l`.c $(LEXER_FILE)
	@$(GCC) $(CFLAGS) $(INCLUDES) -c `basename $^ .l`.c -o $@
	@rm `basename $^ .l`.c

%.o:%.y
	echo "compiling $^"
	$(LEMON) -q $^
	$(GCC) -g $(INCLUDES) -c `dirname $^`/`basename $^ .y`.c -o $@
	rm `dirname $^`/`basename $^ .y`.c

%.o:%.c
	@echo "compiling $^"
	@$(GCC) $(CFLAGS) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(COMPILER_OBJECTS) $(OUTPUT) *core*

