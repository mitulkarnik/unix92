// This program will illustrate the concept of shared memory and pipes 
// parent process will create the file descriptors and will share the descriptors at shared memory 
// child process will read the data from shared memory and will read the data available in the pipe , child process will be the 
// the consumer program invoked by exec

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
//shared memory headers
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
// pipe header
#include <unistd.h>
int *initialize_shared_memory(int);
void sighup_handler(int);

int main(int argc,char *argv[])
{
	printf("Initializing the system... \n");
	// registering the handler 
//	signal(SIGHUP,sighup_handler);	
	
	int file_des[2];
	// initialize the pipe
	pipe(file_des);
	printf("Printing the file descriptors: %d -- %d \n",file_des[0],file_des[1]);
	// write the data to pipe 
	char string[]="This is the text written by the server to the pipe which will be read by client process \n";
	
	int *shm_addr = initialize_shared_memory(sizeof(file_des));
	printf("Shared memory initialized \n");
	
	//sharing the file descriptors at shared memory 
	*shm_addr = file_des[0];
	*(shm_addr + 1) = file_des[1];
	*(shm_addr + 2) = getpid();
	// debug code 
	printf("SHM Address : %x \n",shm_addr);

	//forking to spawn a new process
	pid_t child_process= fork();
	printf("Child process: %d \n",child_process);
	
	if(child_process == -1)
	{
		perror("Fork error, couldn't spawn the new process");
		exit(3);	
	}	
	printf("Child process: %d \n",child_process);

	if(child_process == 0)
	{
		// start the exec of other process	
		int res = execl("/home/mitul/Documents/c_programs/unix_tutorial/pipe_expclient.out","pipe_expclient.out",NULL);
		printf("Exec result: %d \n",res);
	}else
	{
	
		// closing the read end of the pipe 
		close(file_des[0]);
		write(file_des[1],string,(strlen(string)+1));	
	}
	// wait till the child process terminates 
	int status;
	while(1)
	{
		int stat = waitpid(child_process,&status, WUNTRACED);
		if(stat == -1)
		{
			perror("waitpid error\n");
			exit(10);	
		}
		if(WIFEXITED(status))
		{
			printf("Child exited .. terminating the server process \n");
			exit(0);	
		}
		
	}
	return 0;
}

int* initialize_shared_memory(int shm_size)
{
	printf("Initializing the shared memory \n");
	key_t key = ftok(".",'s');
	int shm_id = shmget(key,shm_size,IPC_CREAT | 0666); // Initialize the shared memory 
	if(shm_id < 0)
	{
		printf("Failed to create shared memory.. \n");
		exit(2);	
	}
	int *shm_address;	
	if((shm_address = (int *)shmat(shm_id,NULL,0)) == (int *)-1)  // Attach shared memory to current address space  
	{
		printf("Failed to attach shared memory to current address space \n");
		exit(3);	
	}
	return shm_address;	
}

void sighup_handler(int signum)
{
	printf("Received SIGHUP: %d \n",signum);
	printf("Client process has terminated, terminating the server process \n");
	
	exit(0);
}
