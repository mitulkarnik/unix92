#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void signal_handler(int signal_number);

int main(int argc,char *argv[])
{
	printf("Initializing the system... \n");
	signal(SIGINT,SIG_IGN);
	signal(SIGALRM,signal_handler);

	alarm(5);
	while(1)
	{
		printf("This is infinite loop :  %d \n",getpid());
		sleep(2);
	}
	return 0;
}

void signal_handler(int signal_number)
{
	switch(signal_number)
	{
		case SIGALRM:
		case SIGHUP:
				printf("Interrupt encountered : Signal Number %d \n",signal_number);
				printf("Continueing the process  \n");
		break;
		default: printf("Signal handler not found.. Executing default handler.. \n");	
		exit(2);
	}
}
