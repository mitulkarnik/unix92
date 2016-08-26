#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#define BUFFSIZE 255

void* terminalReadThread(void*);
void* fileReadThread(void*);
void* fileWriteThread(void*);

char *filename;
char *destinationFilename;

int main(int argc,char *argv[])
{
	puts("System is initializing ... \n");
	// Initialize the pipe file descriptors and share them at shared memory 
	int fileDescriptors[2];
	
	int shmid; 
	key_t key;
	int *shmAddress;
	
	key = ftok(".",'s');
	shmid = shmget(key,(sizeof(int)*2),IPC_CREAT | 0666);
	shmAddress = (int *)shmat(shmid,NULL,0);   
	int pipeDescriptors[2];
	pipe(pipeDescriptors);
	// pipe is initialized now share the pipe at shared memory 
	
	*shmAddress = pipeDescriptors[0];
	*(shmAddress + 1) = pipeDescriptors[1];
	
	
	shmdt(shmAddress);
	//Now create the threads for performing the operation 
	pthread_t *tReadThread,*fReadThread,*fWriteThread;

	filename = argv[1];	
	destinationFilename = (char *)calloc(strlen("pipeThreadOutput.txt"),sizeof(char));
	strcpy(destinationFilename,"pipeThreadOutput.txt");

	pthread_create(tReadThread,NULL,&terminalReadThread,NULL);
	pthread_create(fReadThread,NULL,&fileReadThread,NULL);
	pthread_create(fWriteThread,NULL,&fileWriteThread,NULL);

	puts("Main Thread waiting for the child threads for termination \n");	
	pthread_join(tReadThread);
	pthread_join(fReadThread);
	pthread_join(fWriteThread);
	
	puts("Main thread terminating \n");
	return 0;
}

void fileReadThread(void *args)
{
	puts("File reading thread is initiated \n);
	puts("Opening the file for reading \n");
	FILE *fptr = fopen(filename,"r");
	
	// Attaching the shared memory for pipe descriptors 
	key_t key = ftok(".",'s');
	int shmid = shmget(key,(sizeof(int)*2),0444);
	int *shmAddress = (int *)shmat(shmid,NULL,0);
	
	int pipeDescriptors[2];
	pipeDescriptors[0] = *shmAddress;
	pipedescriptors[1] = *(shmAddress + 1);
	
	shmdt(shmAddress);
	
	// close the read end of the pipe 
	close(pipeDescriptors[0]);
        char *readBuffer = (char *)calloc(BUFFSIZE,sizeof(char));
	while(fgets(readBuffer,BUFFSIZE,fptr) != NULL)	
	{
		write(pipeDescriptors[1],readBuffer,strlen(readBuffer));
	}
	// close the descriptors and free the memory 
	fclose(fptr);
	close(pipeDescriptors[1]);
	free(readBuffer);
	puts("File Read Thread terminating, Data is written to the pipe for write thread \n");
} 

void fileWriteThread(void *args)
{
	puts("File Write thread, this thread will read the data from pipe and write to the destination file \n");
	FILE *fptr = fopen(destinationFilename,"w");
