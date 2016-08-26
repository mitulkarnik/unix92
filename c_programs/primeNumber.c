#include <stdio.h>
#include <stdlib.h>

int main()
{
	unsigned long long int index,j;
	int *primes;
	int z=1;
	int llimit,ulimit;
	scanf("%d %d",&llimit,&ulimit);
	// allocate the memory for primes 
	primes = (int *)malloc(sizeof(int)*ulimit);
	
	// initializing the primes with 1
	for(index = 0;index < ulimit;index ++)
		primes[index] = 1;
	
	// sieve algorithm 
	for(index = 2;index < ulimit;index ++)
		if(primes[index])
			for(j=index;index * j < ulimit;j++)
				primes[index*j] = 0;
	
	for(index = 2;index < ulimit;index ++)
		if(primes[index] && index >= llimit)
			printf("%llu ",index);
	return 0;	
}
