#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
extern char **getline();
void execute(char **args);

int main(int agrc, const char* argv[]) {

	int i;
	char **args;
	while(1) {
		args = getline();
		for(i = 0; args[i] != NULL; i++) {
				printf("input- %s\n", args[i]);
				if (strcmp(args[0], "exit") == 0) exit(0);/* check for exit*/
				execute(args);/*send args to execute to parse and use execvp*/

		}


	}
}

void execute(char **args){

	pid_t pid;
	int reread=0;
	char* rereadfile;
	int rewrite=0;
	char* rewritefile;
	int fileout;
	int filein;
	int piped;
	int pipek[2];
	char *pipecmd;
	int status;
	int i;
	/*char *pipea[];*/

	for(; args[i]!=NULL; i++){
		if (strcmp(args[i],"<")==0){ /* check for output redirection*/
			args[i]=NULL;
			reread=1;
			rereadfile=args[i+1];

		}
		else if(strcmp(args[i], ">")==0){ /*check for input redirection*/
			args[i]=NULL;
			rewrite=1;
			rewritefile=args[i+1];
		}
		else if(strcmp(args[i], "|")==0){
			piped=1;
			if(pipe(pipek)<0){
				printf("%s\n","Error: pipe failed");
				exit(1);
			}
			pipecmd=args[i+1];
			break;
		}
	}

	pid = fork(); 				/*fork*/
	  if (pid < 0) { /* check pid condition*/
	    printf("%s\n","Error: Fork failed");
	    exit(1);
	  }
	  else if (pid==0){
		  if(reread>0){/*input redirection */
				printf("%s\n","rf");
				fileout=open(rereadfile, O_RDONLY);
				if(fileout<0){ /* if can't open file, close*/
					 printf("%s\n","Error reading file");
					 exit(1);
				 }

			 dup2(fileout, 0);/* copy the input and redirect it to command*/
			 printf("&s\n", "dup2");
			 close(fileout);
		  }

		  if(rewrite>0){/* output redirection*/
				printf("%s\n","wt");
				filein=open(rewritefile, O_WRONLY | O_TRUNC | O_CREAT, 0644); /*open file*/
				if(filein<0){/* if can't open file, close*/
	 				  printf("%s\n","2Error writing file");
	 				  exit(1);
	 			  }
				dup2(filein,1); /* copy command output into file*/
			  	printf("&s\n", "dup2");
			  	close(filein);
		  }

		  if(piped>0){ /*pipe*/
			  printf("%s\n", "piping");
			  while ((dup2(pipek[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}			  close(pipek[1]);
			  /*while (read(pipek[0], &buf, 1) > 0)
				  write(STDOUT_FILENO, &buf, 1);
			  write(STDOUT_FILENO, "\n", 1);*/
			  close(pipek[0]);
			  execl(args[0], args);
		  }
		 /* if(pipecmd!=0){
			  close(pipek[0]);
			  dup2(pipek[1], 1);
			  close(pipek[1]);
		  }*/

		  if( execvp(*args, args) < 0) { /* handle invalid command arguments*/
		        printf("ERROR: %s is not a valid command\n", args[0]);
		        exit(1);
		  }
	  }
		  else{

		          if(piped==1){ /* finish piping */
		        	 close(pipek[1]);
		      }
		          /*close(pipek[0]);
		          close(pipek[1]);*/
		          while(wait(&status) != pid); /* wait for child to die*/
	  }
}



