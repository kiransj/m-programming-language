GCC=gcc
AR=ar
CFLAGS := -g -m32 
WARNINGS:= -Wall
LEMON:=./lemon/lemon.out

FEATURES := -DENABLE_KEY_VALUE #-DPRINT_BYTE_CODE

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
SOURCE += test/main.c

M_LIB:=libM.a
LD_FLAGS := -m32 -L.  

COMPILER_OBJECTS :=
COMPILER_OBJECTS += $(GRAMMER_FILE:.y=.o)
COMPILER_OBJECTS += $(LEXER_FILE:.l=.o)
COMPILER_OBJECTS += $(SOURCE:.c=.o)

INCLUDES:=  -Iinc -Isrc 

OUTPUT:=a.out

all: $(OUTPUT)

$(OUTPUT): $(COMPILER_OBJECTS) 
	@echo "building $@"
	@$(GCC) $(CFLAGS) $(COMPILER_OBJECTS) $(LD_FLAGS)  -o $@

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
	@$(GCC) $(CFLAGS)  $(INCLUDES) -c `dirname $^`/`basename $^ .y`.c -o $@
	@rm `dirname $^`/`basename $^ .y`.c

%.o:%.c
	@echo "compiling $^"
	@$(GCC) $(CFLAGS) $(FEATURES) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(COMPILER_OBJECTS) $(OUTPUT) $(M_LIB) *core*

