#include "lists.h"

List * listConstruct() {
    List * list = calloc(1, sizeof(List));

    if (list == NULL) {
        fprintf(stderr, "Memory allocation error: Could not allocate memory for a new list\n");
        abort();
    }

    list->next = NULL;
    
#if IS_DOUBLE
    list->prev = NULL;
#endif

    return list;
}

List * listConstructWith(list_t value) {
    List * list = listConstruct();
    list->value = value;

    return list;
}

#if IS_DOUBLE
List * listStart(List * list) {
    while (list->prev != NULL) {
        list = list->prev;
    }
    return list;
}
#endif

List * listEnd(List * list) {
    while (list->next != NULL) {
        list = list->next;
    }
    return list;
}

List * listAppend(List * list, list_t value) {
    list = listEnd(list);
    list->next = listConstructWith(value);
    
#if IS_DOUBLE
    list->next->prev = list;
#endif

    return list->next;
}


#if IS_DOUBLE
List * listPrepend(List * list, list_t value) {
    list = listStart(list);
    list->prev = listConstructWith(value);
    list->prev->next = list;
    return list->prev;
}
#endif

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
