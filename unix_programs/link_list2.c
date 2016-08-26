#include <stdio.h>
#include <stdlib.h>

struct item{
	int data1;
	int data2;
};
typedef struct item item;

void display_struct(struct item *item1)
{
	printf("Displaying the struct: ");
	printf("Data 1: %d \n",item1->data1);
	printf("Data 2: %d \n",item1->data2);
	printf("\n");
}

void init_struct(struct item *item1)
{
	item1->data1 = 20;
	item1->data2 = 30;
	display_struct(item1);
}


int main(int argc,char *argv[])
{
        item *item1 = (item*) malloc(sizeof(struct item));
	printf("Address: %x \n",item1);
	
	init_struct(item1);
	display_struct(item1);
	
	item *item2 = (item*) malloc(sizeof(struct item));
	printf("Address: %x \n",item2);
	init_struct(item2);
	display_struct(item2);
	
	return 0;
}
