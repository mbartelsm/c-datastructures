#ifndef __DICT_CORSARIUS
#define __DICT_CORSARIUS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef val_t
 #define val_t int
#endif


typedef struct StrDictEntry {
    char* key;
    val_t val;
    int isDel;
} StrDictEntry;

typedef struct StrDict {
    size_t count;
    size_t size;
    StrDictEntry** table;
} StrDict;

/**
 * Inserts the specified value under the provided key into the dictionary
 * If the key is already present, the value is overwritten and the old
 * value is returned, otherwise the provided value is returned
 */
val_t strDictInsert(StrDict* dict, char* key, val_t value);

/**
 * Attempts to retrieve the value at the provided key into the value pointer
 * If successful it will return a true value, if it doesn't find the key it
 * will instead return false
 */
int strDictSearch(StrDict* dict, char* key, val_t* value);

/**
 * Attempts to delete an entry with the provided key
 * If the key is found, it will be deleted and this will return true
 * Otherwise it will return false
 */
int strDictRemove(StrDict* dict, char* key);

/**
 * Allocates a new dictionary of the provided size
 */
StrDict * strDictNewWithSize(size_t size);

/**
 * Safely frees the dictionary
 */
void strDictFree(StrDict* dict);

#endif
