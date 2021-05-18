#include "dict.h"

#define HASHING_PRIME 1099511628211UL

/**
 * Allocates a new dictionary entry with the provided key and values
 */
StrDictEntry* strDictEntryNew(char* key, val_t value);

/**
 * Frees a given dictionary entry
 */
void strDictEntryFree(StrDictEntry* entry);

/** 
 * Hashes a string using the FNV-1a algorithm
 * Adapted from http://isthe.com/chongo/tech/comp/fnv/
 */
size_t hashStr(char * str, size_t size);

/** 
 * Hashes a string using a variant of the FNV-1a algorithm that
 * employs a different offset and ensures the hash is odd
 * Adapted from http://isthe.com/chongo/tech/comp/fnv/
 */
size_t hashStrOdd(char * str, size_t size);

/**
 * Produces a sequence of hashes that ensures a full permutation over
 * the range [0, size) with i between [0, size) so long as size is a power
 * of 2
 */
size_t probeHash(char* key, size_t size, size_t i);


StrDictEntry* strDictEntryNew(char* key, val_t value) {
    StrDictEntry* entry = malloc(sizeof(StrDictEntry));
    entry->key = key;
    entry->val = value;

    return entry;
}


void strDictEntryFree(StrDictEntry* entry) {
    free(entry);
}


StrDict * strDictNewWithSize(size_t size) {
    StrDict* dict = malloc(sizeof(StrDict));

    // Ensure table pointers are set to NULL
    dict->table = calloc(size, sizeof(StrDictEntry*));
    dict->count = 0;
    dict->size = size;

    return dict;
}


void strDictFree(StrDict* dict) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->table[i] != NULL) {
            strDictEntryFree(dict->table[i]);
            dict->table[i] = NULL;
        }
    }
    free(dict->table);
    free(dict);
}


val_t strDictInsert(StrDict* dict, char* key, val_t value) {

    if (dict->count == dict->size) {
        fprintf(stderr, "StrDict is full. Cannot insert entry with key %s\n", key);
        abort();
    }

    // Calculate hashes
    size_t primaryHash = hashStr(key, dict->size);
    size_t secondaryHash = hashStrOdd(key, dict->size);

    int i = 0;
    size_t hash;
    val_t result = value;

    while (i < dict->size) {
        hash = (primaryHash + i * secondaryHash) % dict->size;

        // If no entry is present, make a new one
        if (dict->table[hash] == NULL) {
            dict->table[hash] = strDictEntryNew(key, value);
            break;
        }

        // If entry is marked as deleted, dispose and make a new one
        if (dict->table[hash]->isDel) {
            strDictEntryFree(dict->table[hash]);
            dict->table[hash] = strDictEntryNew(key, value);
            break;
        }

        // If entry with given key exists, overwrite
        if (strcmp(dict->table[hash]->key, key) == 0) {
            result = dict->table[hash]->val;
            dict->table[hash]->val = value;
            break;
        }

        // If current hash key is in use, try the next one
        i++;
    }

    if (i == dict->size) {
        fprintf(stderr, "Table overflow, cannot insert entry with key %s\n", key);
        abort();
    }

    // Update table size
    dict->count++;

    return result;
}


int strDictSearch(StrDict* dict, char* key, val_t* value) {

    // Calculate hashes
    size_t primaryHash = hashStr(key, dict->size);
    size_t secondaryHash = hashStrOdd(key, dict->size);

    int i = 0;
    size_t hash;

    while (i < dict->size) {
        hash = (primaryHash + i * secondaryHash) % dict->size;

        StrDictEntry* entry = dict->table[hash];

        // If no entry is present, make a new one
        if (entry == NULL) {
            return 0;
        }

        // If entry with given key exists, get value
        if (!(entry->isDel) && strcmp(entry->key, key) == 0) {
            *value = entry->val;
            return 1;
        }

        // If current hash key is in use, try the next one
        i++;
    }

    return 0;
}


int strDictRemove(StrDict* dict, char* key) {

    // Calculate hashes
    size_t primaryHash = hashStr(key, dict->size);
    size_t secondaryHash = hashStrOdd(key, dict->size);

    int i = 0;
    size_t hash;

    while (i < dict->size) {
        hash = (primaryHash + i * secondaryHash) % dict->size;

        StrDictEntry* entry = dict->table[hash];

        // If no entry is present, make a new one
        if (entry == NULL) {
            return 0;
        }

        // If entry with given key exists, get value
        if (!(entry->isDel) && strcmp(entry->key, key) == 0) {
            entry->isDel = 1;
            return 1;
        }

        // If current hash key is in use, try the next one
        i++;
    }

    return 0;
}


size_t hashStr(char * str, size_t size) {

    // Offset
    unsigned long hash = 14695981039346656037UL;

    // Hash string
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        hash *= HASHING_PRIME;
        hash ^= c;
    }

    // Truncate to size
    return hash % size;
}


size_t hashStrOdd(char * str, size_t size) {

    // Offset
    unsigned long hash = 3300246481284065416UL;

    // Hash string
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        hash *= HASHING_PRIME;
        hash ^= c;
    }

    // Truncate to size and make odd
    return (hash % size) | 1UL;
}

size_t probeHash(char* key, size_t size, size_t i) {
    if (i == 0) {
        return hashStr(key, size);
    }
    return (hashStr(key, size) + i * hashStrOdd(key, size)) % size;
}
