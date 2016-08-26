#include <stdio.h>
#include <stdlib.h>

// Node for linked list
struct node{
	int data;
	struct node *next;
	struct node *prev;
};
typedef struct node Node;

// Functions for linked list operations 
Node* initializeNode(int);		// $1 int data for node 
void addNode(Node **,Node **,Node**);	// $1 start ptr, $2 last ptr, $3 node ptr 
void traverse(Node **);			// $1 start ptr 
void addNodePos(Node **,Node**,int,int);	// $1 start ptr, $2 Node ptr, int position, int actual length of the list 
Node* remNode(Node **,int);		// $1 start ptr, $2 data to be removed from the linked list 

int main(int argc,char *argv[])
{
	puts("Initializing the Doubly circular linked list... ");
	Node *start,*last;		// start and last pointer of the linked list 
	start = last = NULL;		// initiate them to NULL
	int listLength = 0;		//Initialize the list length to maintain the count of nodes attached 
	int index;
	int initCount = 10;
	Node *node;	
	// Create the linked list 
	for(index = 0;index < initCount;index ++)
	{
		node = initializeNode(index*10);
		addNode(&start,&last,&node);
		listLength++;	
	}
	traverse(&start);
	// Adding the data node at specified position 
	node = initializeNode(500);
	addNodePos(&start,&node,1,listLength++);
	traverse(&start);
	puts("terminating the program"); 
	return 0;
}

void addNodePos(Node **start,Node **node,int pos,int listLength)
{
	if(pos > listLength)
	{
		puts("Invalid position, Node cannot be inserted at the specified postion, position greater than actual list length");
		return;	
	}
	Node *currentNode = *start;
	int index = 1;
	while(index++ != pos)
		currentNode = currentNode->next;
	// fetched the target node now inserting the node at the linked list 
	(*node)->next = currentNode->next;
	currentNode->next->prev = *node;
	(*node)->prev = currentNode;
	currentNode->next = *node;
}
void traverse(Node **start)
{
	puts("Traversing and displaying the doubly cicular linked list ");
	
	Node *startPtr = *start;
	Node *currentNode = *start;
	
	do{
		printf("%d ",currentNode->data);
		currentNode = currentNode->next;
	}while(currentNode != startPtr);
	puts("\nDone with the Doubly circular linked list ");
}

Node *initializeNode(int data)
{
	Node *node = (Node *)malloc(sizeof(Node));
	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	
	return node;	
}

void addNode(Node **start,Node **last,Node **node)
{
	// add the node the end of linked list 
	if(*start == NULL)
	{
		// The inbound node is the first node of the linked list 
		*start  = *node;
		*last = *start;	
	}else{
	// attach the node to next of the currently last node modify the last pointer and accordingly the next and prev of boundary nodes
		(*node)->next = (*start);
		(*last)->next = (*node);
		(*node)->prev = *last;
		(*start)->prev = *node;
		*last = *node;
	}
}
