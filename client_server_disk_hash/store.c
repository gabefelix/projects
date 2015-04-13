

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> /* for write */
#include "store.h"
#include "hash.h"

const unsigned int magic_marker = 0xDEADD00D;
const unsigned int empty_marker = 0x0;
const unsigned int marker_length = 4;
static int file_fd;

/**
 * Checks for collision at index key and performs
 * linear probing return the index of an empty slot
 * if the table is not full.
 *
 * @param  fd  the file descriptor containing the table
 * @param  key [table index]
 * @return     [the empty slot or -1 if table is full]
 */
int get_free_slot(int fd, char *key) {
  int read_rv, write_rv, seek_rv;
  int line_offset;
  int table_size, line_size;
  int file_entries;
  size_t probe_key, collision_index;
  unsigned int marker;

  /* read table size and length*/
  lseek(fd, 0, SEEK_SET);
  read(fd, &table_size, sizeof(int));
  read(fd, &line_size, sizeof(int));
  file_entries = table_size + 1;
  printf("%s%d\n", "Table size read: ", table_size);
  printf("%s%d\n\n", "Line size read: ", line_size);

   /* hash the key */
  size_t hash_key = hash(key, table_size);
  printf("%s%s%s%zu\n", "Key '", key, "' hashes to entry: ", hash_key);


  /* seek to correct line entry using the hashed key*/
  line_offset = (hash_key) * line_size;
  seek_rv = lseek(fd, line_offset, SEEK_SET);
  printf("%s%d\n", "Seeked: ", seek_rv);
  read(fd, &marker, sizeof(int));
  printf("%s%u\n", "Marker: ", marker);


  probe_key = hash_key;

  collision_index = probe_key;
  while(marker == magic_marker) {
    /* hash collision, entry is marked by magic_marker marker.
     * beging probing */
    probe_key++;
    if(probe_key == file_entries) {
      /* wraparound when the last entry in the table is reached */
      printf("\n%s\n", "====================================");
      printf("%s\n", "Reached end of table, wrapping around");
      printf("%s\n", "====================================");

      probe_key = 1;
    } else if(probe_key == collision_index) {
      /* table is full */
      return -1;
    }
    printf("\n%s%zu%s\n", "Line entry occupied. Probing line ", probe_key, "...");

    line_offset = (probe_key) * line_size;
    seek_rv = lseek(fd, line_offset, SEEK_SET);
    printf("%s%d\n", "Seeked: ", seek_rv);
    read(fd, &marker, sizeof(int));
    printf("%s%u\n", " ==Line marker: ", marker);
  }

  return probe_key;
}

/**
 * This function returns a file descriptor to the given
 * file name if it exists. Otherwise a new file is created
 * with "size" number of entries all initialized to 0x00000000
 * and a -1 is returned
 * .
 * @param  file   [name of file to open]
 * @param  length [description]
 * @param  size   [number of entries in the hash table]
 * @return        [the file descriptor]
 */
int initialize(char *file, int length, int size) {
  int fd;
  int i;
  int rv;
  fd = open(file, O_WRONLY);
  if(fd == -1) {
    /* file does not exist. create it with correct permissions.
     * and set all entries to 0 */
    fd = open(file, O_CREAT | O_WRONLY, 0640);
    printf("%s%s%s%s%s%d\n", "File ", file, " does not exist. Creating ", file, " with size: ", size);
    /* write header metadata*/
    write(fd, &size, sizeof(size));
    write(fd, &length, sizeof(length));
    lseek(fd, length*(size+1)-marker_length, SEEK_SET);
    rv = write(fd, &empty_marker, sizeof(int));
    if(rv == -1) {
      perror("error");
      return -1;
    }
    return -1;
  }

  printf("%s%s%s\n", "File ", file, " opened.");

  return fd;
}

/**
 * inserts @param value using @key as an index into the hashtable
 * and return the location. Otherwise return -1 if table is full.
 *
 * @param  key    the key associated with this value
 * @param  value  to be inserted
 * @param  length of @param value
 * @return        the index location or -1 if table is full
 */
int insert(char *key, void *value, int length) {
  int read_rv, write_rv, seek_rv;
  int table_slot;

  if(key == NULL || value == NULL) return -1;

  printf("\n%s\n", "==============================");
  printf("%s%s\n%s%s\n%s%d\n", "Key: ", key, "Value: ", value, "Length: ", length);
  printf("%s\n\n", "==============================");

  int fd = open("table.dat", O_RDWR);

  table_slot = get_free_slot(fd, key);
  if(table_slot < 0) {
    return -1;
  } else {
    /* line is free. write entry */
    printf("\n%s%d%s\n", "Slot ", table_slot, " is free.");
    printf("\n%s%d\n", " =Writing entry at line ", table_slot);

    lseek(fd, -(sizeof(int)), SEEK_CUR);
    write_rv = write(fd, &magic_marker, sizeof(magic_marker));
    printf("%s%d\n", "  ==magic_marker bytes written: ", write_rv);
    write_rv = write(fd, key, strlen(key));
    printf("%s%d\n\n", "  ==Key bytes written: ", write_rv);
    write_rv = write(fd, value, length);
    printf("%s%d\n", "  ==Value bytes written: ", write_rv);
  }

  close(fd);
  return table_slot;
}

/**
 * Returns value associated to key. Value is written
 * to @param value and the length to @param length.
 *
 * @param  key    [key that points to value]
 * @param  value  [if value exists we will write to this]
 * @param  length [length will be written if @param value is found]
 * @return        return value slot location or -1 if not found
 */
int fetch (char *key, void *value, int *length) {
  int read_rv, write_rv, seek_rv;
  int line_offset;
  int table_size, line_size;
  int file_entries;
  int key_length = strlen(key);
  int value_length;
  char key_read[key_length];
  size_t probe_key, original_hash;
  unsigned int marker;

  if(key == NULL) {
    printf("%s", "NULL");
    return -1;
}
  int fd = open("table.dat", O_RDONLY);
  if(fd == -1) {
    perror("error");
    return -1;
  }

  /* read table size and length*/
  lseek(fd, 0, SEEK_SET);
  read(fd, &table_size, sizeof(int));
  read(fd, &line_size, sizeof(int));
  file_entries = table_size + 1;
  //printf("%s%d\n", "Table size read: ", table_size);
  //printf("%s%d\n\n", "Line size read: ", line_size);

   /* hash the key */
  size_t hash_key = hash(key, table_size);
  //printf("%s%s%s%zu\n", "Key '", key, "' hashes to entry: ", hash_key);

  original_hash = hash_key;
  do {
    /* seek to the beginging of an entry and read its marker */
    line_offset = (hash_key) * line_size;
    seek_rv = lseek(fd, line_offset, SEEK_SET);
    read_rv = read(fd, &marker, sizeof(unsigned int));

    /* key hashes to an empty entry therefor value does not exist */
    if(marker == 0) return -1;

    /* Read key */
    read_rv = read(fd, &key_read, key_length);
    key_read[key_length] = '\0'; /* just to be safe*/

    value_length = line_size - (key_length + marker_length);
    char buffer[value_length];

    if(strcmp(key_read, key) != 0) {
      /* different key at this slot. start probing */
      //printf("%s\n", "Key-Value pair not at this slot. Probing..");
      hash_key++;
      if(hash_key == table_size+1) hash_key = 1; /* wraparound*/
      if(original_hash == hash_key) return -1; /* searched entire table */

      memset(key_read, 0, key_length);
    } else {
       // printf("%s\n", "Key-Value pair found.");
        /* Calculate length of value and read it */

        read_rv = read(fd, &buffer, value_length);
        buffer[value_length+1] = '\0';

        memcpy(value, buffer, strlen(buffer)+1);
        *length = strlen(buffer);
        return hash_key;
    }

  } while(marker != 0);

  close(fd);
  return -1;
}

/**
 * Searchs hash table location for value that matches @param key
 * and returns true (1) if found false (-1) Otherwise
 * @param  key [description]
 * @return     [description]
 */
int probe (char *key) {
  int read_rv, write_rv, seek_rv, fetch_rv;
  char dummy_value[] = "";
  int dummy_length;

  fetch_rv = fetch(key, dummy_value, &dummy_length);
  return fetch_rv;
}

/**
 * Delete entry associated with key if found.
 *
 * @param  key [key entry we want to delete]
 * @return     [0 for succesful deletion and -1 if not found]
 */
int delete (char *key) {
  int probe_rv, seek_rv, read_rv, write_rv;
  int insert_rv;
  int table_size, line_length;
  int file_entries;
  unsigned int read_marker;
  char dummy_value[] = "";
  int dummy_length;
  int fd;

  probe_rv = fetch(key, dummy_value, &dummy_length);
  if(probe_rv > 0) {
    /* key found */
    fd = open("table.dat", O_RDWR);
    /* read table size and length*/
    lseek(fd, 0, SEEK_SET);
    read(fd, &table_size, sizeof(int));
    read(fd, &line_length, sizeof(int));
    file_entries = table_size + 1;

    printf("%s%d\n", "key found at slot: ", probe_rv);
    seek_rv = lseek(fd, line_length * probe_rv, SEEK_SET);

    /* zero out entry in 4 */
    for (int i = 0; i < line_length / marker_length; ++i) {
      write_rv = write(fd, &empty_marker,  sizeof(int));
    }

    void *temp_buffer[line_length];


    /* tidy up table by moving up any nonempty slots*/
    do {
      /* check the next slot. if empty we are done */
      read_rv = read(fd, &read_marker, sizeof(unsigned int));
      if(read_marker == 0) return 0;
      if(probe_rv == table_size) return 0;
      printf("%s%u\n", "Marker: ", read_marker);

      /* save this entry*/
      lseek(fd, -marker_length, SEEK_CUR);
      read(fd, &temp_buffer, line_length);

      /** then seek back to zero it out */
      lseek(fd, -line_length, SEEK_CUR);
      for (int i = 0; i < line_length / marker_length; ++i) {
        write_rv = write(fd, &empty_marker,  sizeof(int));
      }

      /* seek back to prev entry and insert*/
      lseek(fd, -(line_length*2), SEEK_CUR);
      write(fd, &temp_buffer, line_length);

      /* return back */
      lseek(fd, (line_length), SEEK_CUR);

      memset(temp_buffer, 0, line_length);
      probe_rv++;
    } while(read_marker != 0);

  } else {
      /* key was not found */
      printf("%s\n", "Unable to delete. Key does not exist.");
      close(fd);
      return -1;
  }
  /* succesful deletion */
  close(fd);
  return 0;
}
