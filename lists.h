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

/**
 * Creates a new empty list, value is set to bitwise 0.
 */
List * listConstruct();

/**
 * Creates a new list with the provided value
 */
List * listConstructWith(list_t value);

/**
 * Returns the last entry in the list
 */
List * listEnd(List * list);

/**
 * Inserts a new value at the end of the list, the tail
 */
List * listAppend(List * list, list_t value);

/**
 * Inserts a new value at the current position in the list, pushing the
 * old value towards the end
 */
List * listInsert(List * list, list_t value);

/**
 * Removes and returns the next value in the list
 * If next is NULL, it will cause a runtime error and abort
 */
list_t listTakeNext(List * list);

/**
 * Frees the entire list
 * If IS_DOUBLE is false, it only frees from the current node until the end,
 * ignoring previous nodes.
 */
void listFree(List * list);

/**
 * Prints the entire list
 * If IS_DOUBLE is false, it only prints from the current node until the end,
 * ignoring previous nodes.
 */
void listPrint(List * list);

#if IS_DOUBLE

/**
 * Returns the first entry ni the list, the head
 * This is only avaliable if IS_DOUBLE is true
 */
List * listStart(List * list);

/**
 * Inserts a new value at the beginning of the list
 * This is only avaliable if IS_DOUBLE is true
 */
List * listPrepend(List * list, list_t value);

/**
 * Removes and returns the current value in the list
 * This is only avaliable if IS_DOUBLE is true
 */
list_t listTake(List * list);

#endif

#endif
