#ifndef __LISTS_CORSARIUS
#define __LISTS_CORSARIUS

#include <stdlib.h>
#include <stdio.h>

#ifndef list_t
#define list_t int
#endif

#ifndef IS_DOUBLE
#define IS_DOUBLE 1
#endif

typedef struct List {
    struct List * next;
    
#if IS_DOUBLE
    struct List * prev;
#endif
    list_t value;
} List;

List * listConstruct();
List * listConstructWith(list_t value);
List * listEnd(List * list);
List * listAppend(List * list, list_t value);
List * listInsert(List * list, list_t value);
list_t listTakeNext(List * list);
void listFree(List * list);
void listPrint(List * list);

#if IS_DOUBLE
List * listStart(List * list);
List * listPrepend(List * list, list_t value);
list_t listTake(List * list);
#endif

#endif