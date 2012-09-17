#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <stdio.h>
#include "util.h"
extern unsigned int line_number;

const char* token_to_str(int token);
typedef enum
{
    IDENTIFIER_TYPE_UNKNOWN_START,
    IDENTIFIER_TYPE_NUMBER,
    IDENTIFIER_TYPE_STRING,
    IDENTIFIER_TYPE_ARGUMENT,
    IDENTIFIER_TYPE_VARIABLE,
    IDENTIFIER_TYPE_REGISTER,
    IDENTIFIER_TYPE_OBJECT,
    IDENTIFIER_TYPE_MAP_ELEMENT,
    IDENTIFIER_TYPE_UNKNOWN_END,
}IdentifierType;
const char* IdentifierType_to_str(IdentifierType);

struct _Identifier;
typedef struct _Identifier *Identifier;

typedef struct _Object
{
    char type[16];
    int num_refs;
    void (*obj_delete)(void *ptr);
    void *priv_data;
}*Object;

typedef struct _MapElement
{
    char *map_name;
    char *element_name;
}*MapElement;

MapElement MapElement_Create(const char *map_name, const char *element_name);
void MapElement_Delete(MapElement m);

typedef struct _ArrayElement
{
    char *array_name;
    struct _Identifier *idx;
}*ArrayElement;

ArrayElement ArrayElement_Create(const char *array_name, Identifier idx);
void ArrayElement_Delete(ArrayElement m);

struct _Identifier
{
    IdentifierType type;
    union
    {
        char            *str;
        char            *variable_name;
        int             number;
        int             argument_number;
        int             register_number;
        Object          obj;
        MapElement      map_element;
        ArrayElement    array_element;
    }u;
};

void Identifier_to_str(Identifier id, char * const buffer, const int size);


Identifier Identifier_Create(void);
void Identifier_Destroy(Identifier A);
Identifier Identifier_Clone(Identifier A);
void Identifier_Copy(Identifier dest, Identifier src);

void Identifier_Free(Identifier t);
Identifier Identifier_NewString(const char *str);
Identifier Identifier_NewFloat(const double real);
Identifier Identifier_NewArgument(const int argument_number);
Identifier Identifier_NewVariable(const char *variable_name);
Identifier Identifier_NewInteger(const int number);
Identifier Identifier_NewRegister(const int number);
Identifier Identifier_NewObject(Object);
Identifier Identifier_NewMap(const char *map_name, const char *element_name);

inline void Identifier_SetInt(Identifier a, int num);
inline void Identifier_SetFloat(Identifier a, double num);
inline void Identifier_SetString(Identifier a, char *str);
inline void Identifier_SetObject(Identifier dest, Object obj);

typedef struct _IdentifierStack
{
    Identifier *list;
    int top, size;
}*IdentifierStack;

IdentifierStack IdentifierStack_Create(void);
Identifier IdentifierStack_Pop(IdentifierStack);
STATUS IdentifierStack_Push(IdentifierStack, Identifier);
void IdentifierStack_Destroy(IdentifierStack is);

#ifndef YYSTYPE
#define YYSTYPE Identifier 
#endif

YYSTYPE   yylval;

extern FILE *yyin;

int yylex(void);
int yylex_destroy(void);
#endif

