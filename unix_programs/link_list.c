#include <stdio.h>
#include <stdlib.h>

struct link_list_struct{
	int data;
	struct link_list_struct *next_item;
};

typedef struct link_list_struct link_list;

// function declaration 
link_list* initialize_link_list(link_list*,int);
link_list* initialize_list_item(link_list*,int);
link_list* get_item();
void print_link_list(link_list*,int);
void display_item(link_list *);
link_list* add_item(link_list *,link_list,int);

int main(int argc,char *argv[])
{
	printf("Initializing the system \n");
	link_list *start;
	start = initialize_link_list(start,5);
	printf("Link list initialized \n");

	print_link_list(start,5);
	link_list item;
	item.data = 20;
	display_item(&item);

	start = add_item(start,item,0);	
	print_link_list(start,6);

	printf("Terminating the program \n");
	return 0;
} 

link_list* initialize_link_list(link_list *start,int length)
{
	// initialise the first element of the list
	start = get_item();
	start = initialize_list_item(start,0);
	
	// now initialize the nex item of the list and point the previous item to the current item of the list till length 
	int index;
	link_list *current_item=start;
	link_list *item;
	for(index = 1;index < length;index++)
	{
		item = get_item();
		item = initialize_list_item(item,index);
		current_item->next_item = item;
		current_item = item;		 	
	}
	return start;	
}

link_list* initialize_list_item(link_list *item,int data)
{
	item->data = data;
	item->next_item = NULL;
	return item;
}

link_list* get_item()
{
	//link_list item;
        link_list *item_ptr = (link_list*)malloc(sizeof(link_list));
	return item_ptr;	
}

void print_link_list(link_list *start,int length)
{
	int index;
	link_list *list_pointer = start;
	printf("Printing the link list \n");
	for(index =0;index < length;index ++)
	{
		display_item(list_pointer);
		list_pointer = list_pointer->next_item;	
	}
	printf("Link list printing done \n");
}

void display_item(link_list *item)
{
	printf("Link list item: ");
	printf("Data : %d \n\n",item->data);
}	

link_list* add_item(link_list *start,link_list item,int index)
{
	link_list *list_pointer = start;
	int list_index;
	for(list_index = 0;(list_pointer != NULL)&&(list_index < index - 1) ;list_index ++)
	{
		list_pointer = list_pointer->next_item;	
	}
	if(list_pointer != NULL)
	{
		link_list *temp = list_pointer->next_item;
		list_pointer->next_item = &item;
		item.next_item = temp;	
	}else
	{
		printf("Invalid index.. ");	
	}
	print_link_list(start,6);
	return start;
}
