/*
 *Gabriel Velazquez
 * gvelazq3 / 1330468
 * ASG4
 */

#include <stdlib.h>
#include "List.h"

typedef struct nodeObj* node;

struct nodeObj {
  int data;
  node prev;
  node next;
};

typedef struct ListObj {
  node cursor;
  node head;
  node tail;
  int length;
  int index;
} ListObj;

List newList() {
  List list = malloc (sizeof(struct ListObj));
  list->cursor = NULL;
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  list->index = -1;
  return list;
}

// deletes the list by freeing its memory locations
void freeList (List* pL){
  if ((*pL) == NULL) {
    printf("Passing nullpointer to freeList()");
    return;
  }
  clear(*pL);
  free(*pL);
  *pL = NULL;
}

int length(List L){
  if (L == NULL){
    printf("Passing nullpointer to length()");
    return -1;
  }
  return L->length;
}

int getIndex(List L){
  if (L == NULL){
    printf("Passing nullpointer to getIndex()");
    return -1;
  }
  return L->index;
}

int front(List L){
  if (L == NULL){
    printf("Passing nullpointer to front()");
    return -1;
  }
  if (L->length > 0) return L->head->data;
  return -1;
}

int back(List L){
  if (L == NULL){
    printf("Passing nullpointer to back()");
    return -1;
  }
  if (L->length > 0) return L->tail->data;
  return -1;
}

int getElement(List L){
  if (L == NULL){
    printf("Passing nullpointer to getElement()");
    return -1;
  }
  if (L->length > 0 && L->index >=0){
    return L->cursor->data;
  }
  return -1;
}

int equals (List A, List B){
  if (A == NULL || B == NULL){
    printf("Passing nullpointer to equals()");
    return -1;
  }
  node right = A->head;
  node left = B->head;
  while (right != NULL && left !=NULL){
    if(right->data != left->data) return 0;
    right = right->next;
    left = left->next;
  }
  if(right != NULL || left != NULL) return 0;
  return 1;
}

void clear(List L){
  if (L == NULL){
    printf("Passing nullpointer to clear()");
    return;
  }
  node head = L->head;
  while (head != NULL){
    node temp = head->next;
    free(head);
    head = temp;
  }
  L->head = NULL;
  L->tail = NULL;
  L->cursor = NULL;
  L->length = 0;
  L->index = -1;
}


void moveTo(List L, int i){
  if (L == NULL){
    printf("Passing nullpointer to moveTo()");
    return;
  }
  L->cursor = L->head;
  L->index = i;
  if(i >= 0 && i < L->length){
    for(; i > 0; i--){
      L->cursor = L->cursor->next;
    }
  } else {
    L->cursor = NULL;
    L->index = -1;
  }
}


void movePrev(List L){
  if (L == NULL) {
    printf("Passing nullpointer to movePrev");
    return;
  }
  if(L->index > 0 && L->index < L->length){
    L->cursor = L->cursor->prev;
    L->index--;
  } else {
    L->cursor = NULL;
    L->index = -1;
  }
}


void moveNext(List L){
  if (L == NULL) {
    printf("Passing nullpointer to moveNext");
    return;
  }

  if(L->index >= 0 && L->index < L->length - 1){
    L->cursor = L->cursor->next;
    L->index++;
  } else {
    L->cursor = NULL;
    L->index = -1;
  }
}


void prepend(List L, int data){
  if(L == NULL) {
    printf("Passing nullpointer to prepend()");
    return;
  }

  L->length++;

  node node = malloc(sizeof(struct nodeObj));
  node->data = data;
  node->prev = NULL;
  node->next = L->head;

  if (L->head == NULL){
    L->tail = node;
  } else {
    L->head->prev = node;
  }
  L->head = node;

  if (L->index >= 0) L->index++;
}


void append(List L, int data){
  if(L == NULL){
    printf("Passing nullpointer to append()");
    return;
  }

  L->length++;
  node node = malloc(sizeof(struct nodeObj));
  node->data = data;
  node->prev = L->tail;
  node->next = NULL;

  if(L->tail == NULL){
    L->head = node;
  } else {
    L->tail->next = node;
  }
  L->tail = node;
}


void insertBefore(List L, int data){
  if (L == NULL){
    printf("Passing nullpointer to insertBefore()");
    return;
  }

  if (!(L->length > 0 && L->index >= 0)) return;
  L->length++;

  node node = malloc(sizeof(struct nodeObj));
  node->data = data;
  node->prev = L->cursor->prev;
  node->next = L->cursor;
  if(node->prev != NULL){
    L->cursor->prev->next = node;
  } else {
    L->head = node;
  }

  L->cursor->prev = node;
  L->index++;
}


void insertAfter(List L, int data){
  if(L == NULL){
    printf("Passing nullpointerto insertAfter()");
    return;
  }

  if (!(L->length > 0 && L->index >= 0)) return;
  L->length++;

  node node = malloc(sizeof(struct nodeObj));
  node->data = data;
  node->prev = L->cursor;
  node->next = L->cursor->next;

  if(node->next != NULL){
    L->cursor->next->prev = node;
  } else {
    L->tail = node;
  }

  L->cursor->next = node;
}


void deleteFront(List L){
  if(L == NULL){
    printf("Passing nullpointer to deleteFront()");
    return;
  }

  if(L->length > 0){
    L->length--;

    if(L->index == 0) L->cursor = NULL;
    if(L->tail == L->head) L->tail = NULL;

    node k = L->head;
    L->head = L->head->next;
    free(k);
    L->head->prev = NULL;
    if(L->index != -1) L->index--;
  }
}


void deleteBack(List L){
  if(L == NULL){
    printf("Passing nullpointer to deleteFront()");
    return;
  }

  if(L->length > 0){
    L->length--;
    if(L->index == L->length - 1){
      L->cursor = NULL;
      L->index = -1;
    }
    if(L->tail == L->head) L->head = NULL;

    node k = L->tail;
    L->tail = L->tail->prev;
    free(k);
    L->tail->next = NULL;
  }
}


void delete(List L){
  if(L == NULL){
    printf("Passing nullpointer to delete()");
    return;
  }

  if(L->cursor == L->head){
    deleteFront(L);
  } else if (L->cursor == L->tail){
    deleteBack(L);
  }

  if (L->length > 0 && L->index > 0){
    L->length--;
    node k = L->cursor;
    L->cursor->next->prev = L->cursor->prev;
    L->cursor->prev->next = L->cursor->next;
    L->cursor = NULL;
    L->index = -1;
    free(k);
  }
}

void printList (FILE *out, List L){
  if (L == NULL){
    printf("Passing nullpointer to printList()");
    return;
  }
  if (out == NULL){
    printf("Passing nullpointer to printList()");
    return;
  }

  node f = L->head;
  while (f != NULL){
    fprintf(out, "%d ", f->data);
    f = f->next;
  }

}

List copyList(List L){
  if(L == NULL){
    printf("Passing nullpointer to copy()");
    return NULL;
  }
  List copy = newList();
  for(node k = L->head; k != NULL; k = k->next){
    append(copy, k->data);
  }
  return copy;
}



