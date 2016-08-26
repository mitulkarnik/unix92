#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Forward declarations of the functions 
int* initialize_shared_memory(int);

// Main function 
int main(int argc,char *argv[])
{
	printf("Initializing the Client process ...");
	printf("Building the environment for reading the shared memory \n");
	int *shm_address;
	int file_des[2];
	
	shm_address = initialize_shared_memory(sizeof(file_des));
	printf("Printing the shared memory address %x \n",shm_address);
	file_des[0] = *shm_address;
	file_des[1] = *(shm_address + 1);
	pid_t server_process_id = *(shm_address + 2);
	printf("Server processid  %d : Parent process %d \n",server_process_id,getppid());

	//Deattach the shared memory 
	shmdt(shm_address);

	// File descriptors initialized
	// close the write end of the pipe 
	close(file_des[1]);	
	char read_buffer[100];
	read(file_des[0],read_buffer,sizeof(read_buffer));

	// closing the file handle	
	close(file_des[0]);
	
	printf("Data from server at pipe: %s \n",read_buffer);
	// sending the signal to server process for termination 
	//kill(server_process_id,SIGHUP);
	
	printf("Terminating the client process \n");
	return 0;
}

int *initialize_shared_memory(int shmsize)
{
	printf("Initializing the shared memory... \n");
	key_t key = ftok(".",'s');
	printf("IPC key : %d \n",key);

	int shmid = shmget(key,shmsize,0666);
	if(shmid < 0)
	{
		printf("Failed to acquire the shared memory \n");
		exit(5);	
	}
	int *shmaddress = shmat(shmid,NULL,0); // attach the shared memory to current process address space
	if(shmaddress == (int *)-1)
	{
		printf("Failed to attach the shared memory to current process address space \n");
		exit(6);	
	}
	return shmaddress;	
}
