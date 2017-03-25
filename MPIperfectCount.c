// Prime number counter
// Author: Mitsakos, 2017

#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int N; // perfect numbers limit
	int countP = 0; // counter of perfect numbers per node
	int count; // total count
	int i, n; // iterators
	int sum; // sum of n-1 numbers

	// MPI initialization
	MPI_Init(&argc, &argv);
	int me, P;
	double ta, tt; // timer start/stop
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	if (me == 0)
	{
		printf("\nInsert N limit of perfect number search: ");
		scanf("%d", &N);
		ta = MPI_Wtime();
	
		if (N == 0)
		{
			printf("\nInvalid limit N!\n");
			return -1;
		}
	}
	
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (n = me; n <= N; n += P)
	{
		sum = 0;
		for (i = 1; i < n; ++i)
		{
			if (n%i == 0)
			{
				sum += i;
			}
		}

		if (sum == n)
		{
			countP += 1;
		}
	}
	
	MPI_Reduce(&countP, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (me == 0)
	{
		tt = MPI_Wtime();
		printf("\nTotal = %d perfect numbers in [1, %d]\n", count - 1, N); // remove 0 from total
		printf("\nTotal run time: %6fs\n", tt - ta);
	}

	MPI_Finalize();
	return 0;
}