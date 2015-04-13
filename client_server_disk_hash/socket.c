#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#include "store.h"

int do_command(char *command, char *arguments);
int do_initialize(char *file_name, int length, int size);
int do_insert(char *key, void *value, int length);
int do_fetch(char *key, void *value, int *length);
int do_delete(char *key);

const	char *msg_filefail="404:File didn't exit, creating file\n";
const	char *msg_filesuccess="204 OK: File opened\n";
const	char *msg_insert="201 OK: Insert success\n";
const	char *msg_insertfail="401 BAD: Insert failed\n";
const	char *msg_fetch="200: fetch success\n";
const	char *msg_fetchfail="400 BAD: Fetch failed\n";
const	char *msg_del="203 OK: Delete success\n";
const	char *msg_delfail="403 BAD: Delete failed\n";
const char *msg_unknown="LUL WHAT";

int listenfd = 0, connfd = 0;

int main (int argc, char **argv){
	char receive[100];
	char command[20], argument[300];
  struct sockaddr_in serv_addr;
  char send_buffer[1025];
  time_t ticks;
	char * pch;
  int found;

  /* create unknown socket */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(send_buffer, '0', sizeof(send_buffer));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(10732);


	/* bind */
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0) {
		printf("bind failed\n");
		return -1;
	} else {
			printf("bind success - binded to port %d\n",serv_addr.sin_port);
	}

	/* listen */
	listen(listenfd, 10);

	while(1) {
		/* accept socket */
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		/* read */
		read(connfd, receive, sizeof(receive));
		printf("%s%s", "Received: ", receive);
		ticks = time(NULL);
		snprintf(send_buffer, sizeof(send_buffer), "%.24s\r\n", ctime(&ticks));

		/* write */
		write(connfd, send_buffer, strlen(send_buffer));

		/* tokenize input from client */
		  pch=strchr(receive,' ');
	  while (pch!=NULL)
	  {
	    printf ("found at %ld\n",pch-receive+1);
	    found = pch-receive+1;
	    break;
	    pch=strchr(pch+1,' ');
	  }
	  memcpy(command, &receive[0], found-1);
	  memcpy(argument, &receive[found], strlen(receive));

		do_command(command, argument);

		/* close socket*/
		close(connfd);
		sleep(1);
	}

	return 0;
}


/**
 * Compares user input with existing command wrapper
 * @param  command [description]
 * @return         [description]
 */
int do_command(char *command, char *argument) {
	/*This checks for INIT - which calls doinitalize() -> store.h  initalize()*/
	if(strcmp(command, "init")==0 | strcmp(command, "INIT") == 0){
		char * pch;
  		char file_name[100];
  		int size;
  		int length;

  		int i = 1;

  		pch = strtok (argument,"<>/");

		  while (pch != NULL) {
		    if(i==2) {
		      strcpy(file_name, pch);
		    } else if(i==5) {
		      length = atoi(pch);

		    } else if(i==8) {
		    		size = atoi(pch);
		    }

		    pch = strtok (NULL, "<>/");
		    i++;
		  }

		int init_status = do_initialize("table.dat", length, size);
		if(init_status == -1){
			/* file did not exist and was created or error opening file*/
			 write(connfd, msg_filefail, strlen(msg_filefail));
			if(send(listenfd, msg_filefail, strlen(msg_filefail),0) < 0){
				printf("%s\n", "Sending message file failed");
			}
		} else {
			/* file opened succesfully*/
				write(connfd, msg_filesuccess, strlen(msg_filesuccess));
				if(send(listenfd, msg_filesuccess, strlen(msg_filesuccess),0) < 0){
					printf("%s\n", "Sending message file success");
				}
		}
	} else if(strcmp(command, "put")==0 | strcmp(command, "PUT")==0){
			char *pch;
  		char key[100];
  		char value[400];

  		int i = 1;

  		pch = strtok (argument,"<>/");

		  while (pch != NULL) {
		    if(i==2) {
		      strcpy(key, pch);
		    } else if(i==5) {
		    		strcpy(value, pch);
		    }

		    pch = strtok (NULL, "<>/");
		    i++;
		  }

			int insert_status = do_insert(key, value, strlen(value));

			if(insert_status == -1){
				/* unable to insert value into hash table */
				 write(connfd, msg_insertfail, strlen(msg_insertfail));
				if(send(listenfd, msg_insertfail, strlen(msg_insertfail),0) < 0){
					printf("%s\n", "Sending message insert failed");
				}
			} else {
				/* value insert succesfully*/
					write(connfd, msg_insert, strlen(msg_insert));
					if(send(listenfd, msg_insert, strlen(msg_insert),0) < 0){
						printf("%s\n", "Sending message insert success");
					}
			}
	} else if(strcmp(command, "get")==0 || strcmp(command, "GET")==0 ){
			char * pch;
  		char key[100];
  		char value[]="";
  		int length;

  		int i = 1;

  		pch = strtok (argument,"<>/");

		  while (pch != NULL) {
		    if(i==2) {
		      strcpy(key, pch);
		      printf("%s\n", key);
		    }

		    pch = strtok (NULL, "<>/");
		    i++;
		  }
			int get_status = do_fetch(key, value, &length);

			if(get_status == -1){
				/* unable to retrieve value from hash table */
				write(connfd, msg_fetchfail, strlen(msg_fetchfail));
				if(send(listenfd, msg_fetchfail, strlen(msg_fetchfail),0) < 0){
					printf("%s\n", "Sending message fetch failed");
				}
			} else {
					/* value found*/
					write(connfd, msg_fetch, strlen(msg_fetch));
					/* write value found back to client*/
					write(connfd, value, strlen(value));

					if(send(listenfd, msg_fetch, strlen(msg_fetch),0) < 0){
						printf("%s\n", "Sending message fetch success");
					}
			}
	} else if(strcmp(command, "del")==0 | strcmp(command, "DEL")==0 ){

			int del_status = do_delete("Coconuts");
			if(del_status == -1){
					/* unable to delete entry at key location */
					write(connfd, msg_delfail, strlen(msg_delfail));
					if(send(listenfd, msg_delfail, strlen(msg_delfail),0) < 0){
						printf("%s\n", "Sending message fetch failed");
					}
				} else {
					/* entry deleted succesfully*/
						write(connfd, msg_del, strlen(msg_del));

						if(send(listenfd, msg_del, strlen(msg_del),0) < 0){
							printf("%s\n", "Sending message fetch success");
						}
				}
	}

	return -1;
}



/*calls store.c initialize()*/
int do_initialize(char *file_name, int length, int size){
	int status=initialize(file_name, length, size);
	if(status==-1){
		return -1;
	}
	return status;
}

/*calls store.c insert()*/
int do_insert(char *key, void *value, int length){

	int insert_slot = insert(key, value, length);
	if(insert_slot == -1){
		return -1;
	}
	return insert_slot;
}
/*calls store.c fetch()*/

int do_fetch(char *key, void *value, int *length){

	int fetch_slot = fetch(key, value, length);
	if(fetch_slot == -1){
		return -1;
	}
	return fetch_slot;
}

/*calls store.c delete()*/
int do_delete(char *key){

	int deleted_slot = delete(key);
	if (deleted_slot == -1){
		return -1;
	}
	return deleted_slot;
}