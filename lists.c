#include <stdlib.h>
#include <stdio.h>


#define list_t int
#define IS_DOUBLE 1


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


#if IS_DOUBLE
List * listStart(List * list);
List * listPrepend(List * list, list_t value);
list_t listTake(List * list);
#endif


/// Creates a new empty list, value is set to bitwise 0.
List * listConstruct() {
    List * list = calloc(1, sizeof(List));

    if (list == NULL) {
        fprintf(stderr, "Memory allocation error: Could not allocate memory for a new list");
        abort();
    }

    list->next = NULL;
    
#if IS_DOUBLE
    list->prev = NULL;
#endif

    return list;
}

/// Creates a new list with the provided value
List * listConstructWith(list_t value) {
    List * list = listConstruct();
    list->value = value;

    return list;
}

#if IS_DOUBLE
/// Returns the first entry ni the list, the head
/// This is only avaliable if IS_DOUBLE is true
List * listStart(List * list) {
    while (list->prev != NULL) {
        list = list->prev;
    }
    return list;
}
#endif

/// Returns the last entry in the list
List * listEnd(List * list) {
    while (list->next != NULL) {
        list = list->next;
    }
    return list;
}

/// Inserts a new value at the end of the list, the tail
List * listAppend(List * list, list_t value) {
    list = listEnd(list);
    list->next = listConstructWith(value);
    
#if IS_DOUBLE
    list->next->prev = list;
#endif

    return list->next;
}


#if IS_DOUBLE
/// Inserts a new value at the beginning of the list
/// This is only avaliable if IS_DOUBLE is true
List * listPrepend(List * list, list_t value) {
    list = listStart(list);
    list->prev = listConstructWith(value);
    list->prev->next = list;
    return list->prev;
}
#endif

/// Inserts a new value at the current position in the list, pushing the
/// old value towards the end
List * listInsert(List * list, list_t value) {
    List * next = list->next;
    list->next = listConstructWith(value);
    list->next->next = next;
    
#if IS_DOUBLE
    list->next->prev = list;
#endif

    return list->next;
}


#if IS_DOUBLE
/// Removes and returns the current value in the list
/// This is only avaliable if IS_DOUBLE is true
list_t listTake(List * list) {
    list_t value = list->value;
    
    if (list->prev != NULL) {
        list->prev->next = list->next;
    }

    if (list->next != NULL) {
        list->next->prev = list->prev;
    }
    free(list);
    return value;
}
#endif

/// Removes and returns the next value in the list
/// If next is NULL, it will cause a runtime error and abort
list_t listTakeNext(List * list) {
    if (list->next == NULL) {
        fprintf(stderr, "Unexpected NULL reference: Cannot perform listTakeNext(list), list->next is NULL");
        abort();
    }

    list_t value = list->next->value;
    List * next = list->next->next;
    free(list->next);
    list->next = next;

    return value;
}

/// Prints the entire list
/// If IS_DOUBLE is false, it only prints from the current node until the end,
/// ignoring previous nodes.
void listPrint(List * list) {
    
#if IS_DOUBLE
    list = listStart(list);
#endif

    printf("{ ");
    while (list != NULL) {
        printf("%d", list->value);
        if (list->next != NULL) {
            printf(", ");
        }
        list = list->next;
    }
    printf(" }\n");
}

/// Frees the entire list
/// If IS_DOUBLE is false, it only frees from the current node until the end,
/// ignoring previous nodes.
void listFree(List * list) {
    
#if IS_DOUBLE
    list = listStart(list);
#endif

    while (list != NULL) {
        List * next = list->next;
        free(list);
        list = next;
    }
}