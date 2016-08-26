// Program for implementing the binary tree
#include <stdio.h>
#include <stdlib.h>

struct node{
	int data;
	struct node *left;
	struct node *right;
};

typedef struct node Node;

inline Node* getTreeNode(int data);
void createTree(Node **,int,int*);
void attachNode(Node**,Node*);

void inorderTraverse(Node *);


int main(int argc,char *argv[])
{
	puts("Binary tree initializing...");
	Node *root = NULL;
	int length = atoi(argv[1]);
	int *arr = (int *)malloc(sizeof(int)*length);
	int index;
	for(index = 0;index < length;index++)
		*(arr+index) = atoi(argv[index + 2]);

	createTree(&root,length,arr);
	puts("Binary tree intialized ");
	inorderTraverse(root);
	puts("Thank you,, Terminating the program");
	return 0;
}

void inorderTraverse(Node *root)
{
	if(root != NULL){
		inorderTraverse(root->left);
		printf("%d ",root->data);
		inorderTraverse(root->right);
	}
}

void createTree(Node **root,int length,int *array)
{
	// Generating the tree of length 
	int index;
	Node *node;
	for(index = 0;index < length;index++)
	{
		if(*root == NULL)
			*root = getTreeNode(*(array + index));		// processing the root node 
		else
		{
			// attach the node as per BST 
			node = getTreeNode(*(array+index));		
			attachNode(root,node);	
		}
	}
}

void attachNode(Node **root,Node *node)
{
	Node *cnode = *root;
	while(1)
	{
		if(cnode->data >= node->data)
		{
			if(cnode->left == NULL){
				cnode->left  = node;
				break;
			}
			else
				cnode = cnode->left;	
		}else
		{
			if(cnode->right == NULL){
				cnode->right = node;
				break;				
			}
			else
				cnode = cnode->right;		
		}	
	}	
}

inline Node* getTreeNode(int data)
{
	Node *node = (Node *) malloc(sizeof(Node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}


