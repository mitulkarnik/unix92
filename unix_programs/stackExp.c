#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define STACKLENGTH 10

struct stack{
	int *array;
	int tos;
	int length;
};

inline void initializeStack(struct stack *);
void push(struct stack *,int);
int pop(struct stack *);
int getLength(struct stack *);
int getTos(struct stack *,int *);
void displayStack(struct stack *);
void terminal_decor();
void destroyStack(struct stack *);

int main(int argc,char *argv[])
{
	fputs("Initializing the stack frame ",stdout);
	terminal_decor();
	struct stack stackObj;
	initializeStack(&stackObj);
	puts("Stack is initialized...");
	
	register int index;
	for(index = 0;index < 5;index ++)
	{
		push(&stackObj,index);
		if(errno == 1)
		{
			perror("Stack overflow error \n");
			exit(errno);	
		}
	}
	
	displayStack(&stackObj);
	printf("Stack size: %d \n",stackObj.length);
	int tosindex;
	int tosvalue = getTos(&stackObj,&tosindex);
	printf("Current top of stack index: %d value: %d \n",tosindex,tosvalue);
	
	int data;
	for(index = 0;index < 3;index ++)
	{
		data = pop(&stackObj);		
		printf("Popped value: %d \n",data);
		if(data == -1 && errno == 2)
		{
			perror("Stack underflow error \n");
			exit(errno);	
		}	
	}
	displayStack(&stackObj);
	tosvalue = getTos(&stackObj,&tosindex);
	printf("Current top of stack index: %d value: %d \n",tosindex,tosvalue);
	
	destroyStack(&stackObj);	
	puts("Terminating the program ...");
	getchar();
	return 0;
}
	
inline void initializeStack(struct stack *stackObj)
{
	// allocate memory for the array first 
	stackObj->array = (int *)calloc(STACKLENGTH,sizeof(int));
	stackObj->tos = -1;
	stackObj->length = STACKLENGTH;
}

void push(struct stack *stackObj,int data)
{
	// perform error check for stack over flow 
	if(stackObj->tos == STACKLENGTH)
	{
		errno = 1;	
		return;		
	}
	// perform expected operation 
	
	stackObj->tos = stackObj->tos + 1;
	*(stackObj->array + stackObj->tos) = data;
}

int pop(struct stack *stackObj)
{
	// error check for stack underflow error 
	if(stackObj->tos == -1)
	{
		errno = 2;
		return -1;	
	}
	int data = *(stackObj->array + stackObj->tos);
	stackObj->tos = stackObj->tos - 1;
	return data;
}

void displayStack(struct stack *stackObj)
{
	puts("Displaying the stack \n");
	int index = 0;
	while(index <= stackObj->tos)
	{
		printf("%d ",*(stackObj->array + index));
		index++;
	}
	puts("Done with the stack \n");
}

int getTos(struct stack *stackObj,int *tosIndex)
{
	*tosIndex = stackObj->tos;
	return *(stackObj->array + stackObj->tos);
}

int getLength(struct stack *stackObj)
{
	return stackObj->length;
}

void terminal_decor()
{
	fflush(stdout);
	char *c = ". ";
	int limit;
	
	for(limit = 0;limit < 3;limit ++)
	{
		write(STDOUT_FILENO,c,2);
		sleep(1);	
	}
}

void destroyStack(struct stack *stackObj)
{
	free(stackObj->array);
}
