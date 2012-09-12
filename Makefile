GCC=gcc
AR=ar
CFLAGS := -g 
WARNINGS:= -Wall
LEMON:=lemon

FEATURES := -DENABLE_KEY_VALUE -DPRINT_BYTE_CODE

GRAMMER_FILE:=src/grammer.y
LEXER_FILE:=src/lexer.l

SOURCE := src/compiler.c
SOURCE += src/util.c
SOURCE += src/identifier.c
SOURCE += src/executable.c
SOURCE += src/execute_context.c
SOURCE += src/function.c
SOURCE += src/variable_list.c
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
	@rm -f $(M_LIB)
	@$(AR) -q $(M_LIB) $(COMPILER_OBJECTS)
	
%.o:%.l
	@echo "compiling $^"
	@flex --outfile=`dirname $^`/`basename $^ .l`.c $(LEXER_FILE)
	@$(GCC) $(CFLAGS) $(INCLUDES) -c `dirname $^`/`basename $^ .l`.c -o $@
	@rm `dirname $^`/`basename $^ .l`.c

%.o:%.y
	echo "compiling $^"
	@$(LEMON) -q $^
	@$(GCC) -g $(INCLUDES) -c `dirname $^`/`basename $^ .y`.c -o $@
	@rm `dirname $^`/`basename $^ .y`.c

%.o:%.c
	@echo "compiling $^"
	@$(GCC) $(CFLAGS) $(FEATURES) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(COMPILER_OBJECTS) $(OUTPUT) $(TEST_OBJECTS) $(M_LIB) *core*

