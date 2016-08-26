// To calculate all permutaitions of the string 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// swap function for swapping 2 characters 
void swap(char *c1,char *c2)
{
	char temp;
	temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

void permute(char *string, int start,int end)
{
	int index;
	if(start == end)
		printf("%s\n",string);
	else
	{
		for(index = start;index <= end;index ++)
		{
			swap(string+start,string+index);
			permute(string,start+1,end);
			swap(string+start,string + index);
		}
	}	
}

int main(){
	char *string  = (char *) malloc(strlen("ABC"));
	strcpy(string,"ABC");
	permute(string,0,strlen(string)-1);
	return 0;
}

