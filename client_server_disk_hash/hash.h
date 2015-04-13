/**
 * Author: Juan C. Manzo
 * Hash table ADT function prototypes.
 */

#if !defined(_hash_H_INCLUDE_)
#define _hash_H_INCLUDE_

typedef struct HashTable *HashRef;
typedef struct Slot *SlotRef;
/*Constructor/Destructor*/
HashRef newTable(size_t size);
SlotRef newSlot(char *key, int length);
void freeTable(HashRef* pH);
/*************************/

size_t hash(char *key, int size);
size_t getSize(HashRef H);

#endif