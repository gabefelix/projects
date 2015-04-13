/**
 * Hash table by: Juan C. Manzo
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> /* for write */
#include "hash.h"

typedef struct Slot {
  unsigned int magic;
  char *value;
  int length;
} Slot;

typedef struct HashTable {
  int size;
  struct Slot **table;
} HashTable;

SlotRef newSlot(char *value, int length) {
  SlotRef slot = malloc(sizeof(Slot));
  slot->value = value;
  slot->length = length;
  slot->magic = 0xDEADD00D;
  return slot;
}
/**
 * Initializes a new instance of the HashTable
 * @param  size [size of the table]
 * @return      [newly created table]
 */
HashRef newTable(size_t size) {
    int i;
    HashRef H = malloc(sizeof(HashTable));

    H->size = size;
    /* allocate enough memory for size slots */
    H->table = malloc(sizeof(Slot) * size);
    for(i = 0; i < size; i++) {
      H->table[i] = NULL;
    }
    return H;
}

/**
 * Frees the table object
 * @param H [object to be freed]
 */
void freeTable(HashRef* H) {
  int i;

  if(H == NULL) {
    return;
  }

  /*Free lists in hash table*/
  for(i = 0; i < getSize(*H); ++i) {
  }

  /*Free fields*/
  free((*H)->table);
  free(*H);
  *H = NULL;
}

/**
 * Returns size of the hashtable @param H
 * @param  H [the hashtable]
 * @return   [size of table]
 */
size_t getSize(HashRef H) {
  if(H == NULL) {
    return -1;
  }

  return H->size;
}


/**
 * Returns a slot on the hashtable by hasing together
 * the key value.
 * @param  key  [description]
 * @param  size [size of the table]
 * @return      [an index corresponding to a slot in the hash table]
 */
size_t hash(char *key, int size) {
  if(size < 1) {
    return -1;
  }
  size_t h = 0;
  size_t c;

  while((c = *key++)) {
    h = (c + (h << 6) + (h << 16) - h) % size;
  }
  return h;
}

