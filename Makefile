GCC=gcc
AR=ar
CFLAGS := -O2 #-g 
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

M_LIB:=libM.a
LD_FLAGS := -L. -lM 

TEST_FILES := test/main.c

COMPILER_OBJECTS :=
COMPILER_OBJECTS += $(GRAMMER_FILE:.y=.o)
COMPILER_OBJECTS += $(LEXER_FILE:.l=.o)
COMPILER_OBJECTS += $(SOURCE:.c=.o)

TEST_OBJECTS := $(TEST_FILES:.c=.o)	

INCLUDES:=  -Iinc -Isrc 

OUTPUT:=a.out

all: $(OUTPUT)

$(OUTPUT): $(M_LIB) $(TEST_OBJECTS)
	@echo "building $@"
	@$(GCC) $(CLFAGS) $(TEST_OBJECTS) $(LD_FLAGS)  -o $@

$(M_LIB):$(COMPILER_OBJECTS)
	@echo "building $(M_LIB)"
	@$(AR) -q $(M_LIB) $(COMPILER_OBJECTS)
	
%.o:%.l
	@echo "compiling $^"
	@flex --outfile=`basename $^ .l`.c $(LEXER_FILE)
	@$(GCC) $(CFLAGS) $(INCLUDES) -c `basename $^ .l`.c -o $@
	@rm `basename $^ .l`.c

%.o:%.y
	echo "compiling $^"
	@$(LEMON) -q $^
	@$(GCC) -g $(INCLUDES) -c `dirname $^`/`basename $^ .y`.c -o $@
	@rm `dirname $^`/`basename $^ .y`.c

%.o:%.c
	@echo "compiling $^"
	@$(GCC) $(CFLAGS) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(COMPILER_OBJECTS) $(OUTPUT) *core*

