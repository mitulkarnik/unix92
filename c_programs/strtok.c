#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *string = (char *)malloc(sizeof(int)*1024);
	scanf("%s",string);

	char *pattern = ":";
	
	string = strtok(string,pattern);
	printf("%s\n",string);
	string = strtok(NULL,pattern);
	printf("%s\n",string);
	string = strtok(NULL,pattern);
	printf("%s\n",string);
	int a = atoi("02");
	printf("%d\n",a);
	return 0;
}

