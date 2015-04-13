/**
 * Test driver for store.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "store.h"
#include "hash.h"

int main() {
  int fd;
  char raw_buf[2];
  void *value;
  char *key = "jc";
  int length = 64;
  int int_value = 2;
  int int_size = sizeof(int_value);
  char char_value[] = "";
  int char_length;
  int table_size = 10;
  int insert_rv, fetch_rv, prove_rv, delete_rv;
  fd = initialize("table.dat", length, table_size);

  char *insert_value = "An entry";
  int insert_length = strlen(insert_value);

 insert_rv = insert("jc", insert_value, insert_length);
  if(insert_rv == -1) {
    printf("%s\n", "Table is full");
  } else {
      printf("%s%d\n", "Value inserted at index ", insert_rv);
  }

  fetch_rv = fetch("jc", char_value, &char_length);
  if(fetch_rv > 0) {
    printf("%s%d\n", "Value found at index: ", fetch_rv);
    printf("%s%s\n", "Value found: ", char_value);
    printf("%s%d\n", "Value size: ", char_length);
  } else {
    printf("%s\n", "Value not found.");
  }

  prove_rv = probe("jc");
  if(prove_rv > 0) {
    printf("%s%d\n", "Probe found value at index: ", prove_rv);
  } else {
    printf("%s\n", "Probe found no value associated with the key.");
  }

 delete_rv = delete("Coconuts");

  return 0;
}