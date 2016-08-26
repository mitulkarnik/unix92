#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
	printf("Initializing the system... \n");
	
	// variable declarations
	key_t key;
	int shmid;
	pid_t *shm;
	pid_t server_pid;

	// access the shared memory
	key = ftok(".",'s');
	if((shmid = shmget(key,sizeof(pid_t),0666)) < 0)
	{
		printf("Failed to get shared memory access... \n");
		exit(1);	
	}
	// attach the shared memory to current process address space 
	if((shm = shmat(shmid,NULL,0)) == (pid_t *) -1)
	{
		printf("Failed to attach shared memory to current process address space \n");
		exit(2);	
	}	
	// retrieve the process id
	server_pid =  *shm;
	printf("Server process pid : %d \n",server_pid);
	// de-attaching the shared memory
	shmdt(shm);	
		
	// send various signals to server process 
	int index;
	for(index = 0;index < 5;index++)
	{
		kill(server_pid,SIGINT);
		printf("Sent SIGINT to server process \n");
		sleep(2);

		kill(server_pid,SIGHUP);
		printf("Sent SIGHUP to server process \n");
		sleep(2);
		
		kill(server_pid,SIGFPE);
		printf("Sent SIGFPE to server process \n");
		sleep(2);		
	}
	printf("Sending kill signal to client process \n");
	kill(server_pid,SIGKILL);
	
	printf("Terminating the client process \n");
	return 0;
}
