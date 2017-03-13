// Array element search
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int N; // N: size of array A
	int *A; // declaration of array A
	int x; // array element to search for
	int counter = 0; // counts x appearances in A
	int i;

	printf("\nInsert array A size:");
	scanf("%d", &N);

	A = malloc(N * sizeof(int));

	if (A == NULL)
	{
		printf("Invalid size N!\n");
		return -1;
	}

	for (int i = 0; i < N; ++i)
	{
		A[i] = rand()%10000;
	}

	printf("\nInsert X element to search for:");
	scanf("%d", &x);

	for (int i = 0; i < N; ++i)
	{
		if (x == A[i])
		{
			counter += 1;
		}
	}

	printf("\nX is found %d times in total", counter);
	return 0;
}

