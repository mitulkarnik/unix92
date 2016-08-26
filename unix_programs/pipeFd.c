#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define BUFFSIZE 300

int main(int argc,char *argv[])
{
	puts("Initializing the system... \n");
	// declare exit handler for peforming cleanup activity

	// declaring all the required variables 
	FILE *fptr;
	int fileDes[2];	// pipe descriptors 
	char *readBuffer = (char *) calloc(BUFFSIZE,sizeof(char));
	if(readBuffer == NULL)
	{
		perror("Couldn't allocate buffer memory for reading, program cannot continue \n");
		exit(1);	
	}
	pid_t child_pid;

	// open the file for reading 
	fptr = fopen(argv[1],"r");
	printf("File descriptor for opened file %d \n",fileno(fptr));
	
	// initialize the pipe 
	if(pipe(fileDes) == -1)
	{
		perror("Pipe error, terminating \n");
		exit(2);	
	}
	
	printf("Pipe descriptor: %d, %d \n",fileDes[0],fileDes[1]);

	if((child_pid = fork()) == -1)
	{
		perror("Fork error terminating \n");
		exit(3);	
	}
	
	if(child_pid == 0)
	{
		// child processing 
		close(fileDes[1]);	// close the write of the pipe 
		int child_pipe[2];
		if(pipe(child_pipe) == -1)
		{
			perror("Pipe error for 1st child process \n");
			exit(2);	
		}
		pid_t cat_process;
		cat_process = fork();
		if(cat_process == 0)
		{
			// cat process use exec
			close(child_pipe[1]);		// close the write end of the second pipe for cat process
			// need to dplicate the stdin for this process
			close(STDIN_FILENO);
			dup(child_pipe[0]);		// stdin is now the duplicate of child_pipe[0]
			close(child_pipe[0]);		// no longer needed the descriptor 
			
			execlp("cat","cat",NULL); 	
		}else{
			close(child_pipe[0]);	// close the read end of second pipe created 

			while(read(fileDes[0],readBuffer,BUFFSIZE) != 0)
			{
				write(child_pipe[1],readBuffer,strlen(readBuffer));
			}	
			close(child_pipe[1]);
			wait();
		}

	}else
	{
		// parent processing 
		close(fileDes[0]);	// close the read end of the pipe for parent 
		while(fgets(readBuffer,BUFFSIZE,fptr) != NULL)
		{
			// write the contents to the pipe 
			write(fileDes[1],readBuffer,BUFFSIZE);		
		}
		// close the open descriptors 
		fclose(fptr);
		close(fileDes[1]);
	}

	// wait for child process to finish
	wait();
	puts("Termianting the main program \n");
	return 0;
}
