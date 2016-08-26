// Spawn the 3 threads whoch will read 3 different files and will write the data into the master file using locking system 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
	
void terminal_decor();
void* readFile1(void *);
int semaphore_status = 0;
int data_control = 1;
int shmid1,shmid2;

struct thread_routine_args{
	char *filename;
	int fnameLength;
};


int main(int argc,char *argv[])
{
	fputs("Initializing the system",stdout);
	terminal_decor();
	// Initialize the shared memory
	key_t key;
	int shmid;
	char *shmAddr;
	char *masterFilename = "MasterOuputData.txt";
	pid_t cpid;
	
	if((cpid = fork()) < 0)
	{
		perror("Fork error, could not spawn new process\n");
		exit(1);	
	}
	if(cpid == 0)
	{
		// Child processing 
		key = ftok(".",'s');
		if(key <= 0)
		{
			perror("Shared Memory key error");
			exit(2);	
		}
		shmid = shmget(key,strlen(masterFilename),IPC_CREAT | 0666);
		if(shmid < 0)
		{
			perror("Couldn't get shared memory from operating sytem \n");
			exit(3);	
		}
		printf("Shared memory id: %d \n",shmid);
		shmAddr = (char *)shmat(shmid,NULL,0);
		if(shmAddr==NULL)
		{
			perror("Couldn't attach shared memory to the current adress space");
			exit(4);	
		}
		strcpy(shmAddr,masterFilename);	
		puts("Master filename shared at shared location, child process terminating");	
		shmdt(shmAddr);
		key = ftok(".",'b');
		shmid = shmget(key,sizeof(int),IPC_CREAT | 0666);
		printf("Shared memory id: %d \n",shmid);
		int *shmAddr1 = (int *) shmat(shmid,NULL,0);
		*shmAddr1 = strlen(masterFilename);
		shmdt(shmAddr1);
		puts("Masterfilename length copied to shared location");				
		exit(0);	
	}else{
		puts("Waiting for data to be available at shared memory");
		wait();
		// Parent processing 
		// create the master file and start the threads with concurrency control 
		FILE *masterFilep = fopen(masterFilename,"w");
		puts("Master file created");
		fclose(masterFilep);
		pthread_t t1,t2,t3;
		struct thread_routine_args args1,args2,args3;
		args1.filename = "Data1.txt";
		args1.fnameLength = strlen("Data1.txt");
		args2.filename = "Data2.txt";
		args2.fnameLength = strlen("Data2.txt");
		args3.filename = "Data3.txt";
		args3.fnameLength = strlen("Data3.txt");		
	
		pthread_create(&t1,NULL,readFile1,(void *)&args1);
		pthread_create(&t2,NULL,readFile1,(void *)&args2);
		pthread_create(&t3,NULL,readFile1,(void *)&args3);
	
		// Main thread will wait for the child thread to complete 
		pthread_join(t1,NULL);
		pthread_join(t2,NULL);
		pthread_join(t3,NULL);
		
		puts("All child threads terminated, main thread proceeding");
	}
	puts("Removing the shared memory segments");
	shmctl(shmid1,IPC_RMID,NULL);
	shmctl(shmid2,IPC_RMID,NULL);
	puts("Main thread terminating");
	return 0;
}

char* getMasterFilename(int *fnamelength)
{
		key_t key = ftok(".",'b');
		if(key <= 0)
		{
			perror("Shared Memory key error");
			exit(2);	
		}
		int shmid = shmget(key,sizeof(int), 0666);
		if(data_control)
			shmid1 = shmid;

		if(shmid < 0)
		{
			perror("Couldn't get shared memory from operating sytem \n");
			exit(3);	
		}
		int *shmAddr = (int *)shmat(shmid,NULL,0);
		if(shmAddr==NULL)
		{
			perror("Couldn't attach shared memory to the current adress space");
			exit(4);	
		}
		*fnamelength = *shmAddr;
		shmdt(shmAddr);

		key = ftok(".",'s');
		if(key <= 0)
		{
			perror("Shared Memory key error");
			exit(2);	
		}
		shmid = shmget(key,*fnamelength, 0666);
		if(shmid < 0)
		{
			perror("Couldn't get shared memory from operating sytem \n");
			exit(3);	
		}
		shmid2 = shmid;
		data_control--;
		char *shmAddr1 = (char *)shmat(shmid,NULL,0);
		if(shmAddr1==NULL)
		{
			perror("Couldn't attach shared memory to the current adress space");
			exit(4);	
		}
		char *result = (char *)malloc(*fnamelength);
		strcpy(result,shmAddr1);
		shmdt(shmAddr1);
		return result;
}

void* readFile1(void *args)
{
	// connect to shared location to read master filename 
	int fnamelength;
	char *masterFilename = getMasterFilename(&fnamelength);
	char *filename;
	int filenameLength;
	filenameLength = ((struct thread_routine_args *)args)->fnameLength;
	filename = (char *)malloc(filenameLength);
	strcpy(filename,((struct thread_routine_args *)args)->filename);
	
	FILE *fptr = fopen(filename,"r");
	char *readBuffer = (char *)malloc(sizeof(char)*255);
	FILE *mfilep;
	// acquire a lock to master file
	while(1)
	{
		if(!semaphore_status)
		{
			// lock acquired 
			semaphore_status++;
			mfilep = fopen(masterFilename,"a");
			break;	
		}else
			sleep(1);	
	}	
	
	while(fgets(readBuffer,255,fptr) != NULL)
	{
		fputs(readBuffer,mfilep);	
	}
	// release the lock of master file 
	fclose(mfilep);
	fclose(fptr);
	semaphore_status--;
	free(filename);
	free(readBuffer);
	free(masterFilename);
}

void terminal_decor()
{
	fflush(stdout);
	int index;
	char *c = ". ";
	for(index = 0;index < 3;index ++)
	{
		write(STDOUT_FILENO,c,1);
		sleep(1);
	}
}
