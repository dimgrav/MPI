// MPI Max search - collective communication
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int *A, *B, *MAX; // global array, local array, max positions array
	int N, part; // size of A, B
	int localPos; // max position (part)
	int *globalPos; // max position (N)
	int maxPos; // max value position in MAX
	int i, j; // iterators

	int id, P; // MPI rank, size
	double ta, tt; // MPI timer start, stop

	// Initiate MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	if (id == 0)
	{
		printf("\nInsert N, size of A: ");
		scanf("%d", &N);
		// global array setup
		A = malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
		{
			A[i] = rand() % 100;
		}

		part = N / P; // per node
		MAX = malloc(P * sizeof(int));
	}

	MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);

	B = malloc(part * sizeof(int));
	globalPos = malloc(sizeof(int));

	MPI_Scatter(A, part, MPI_INT, B, part, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(MAX, 1, MPI_INT, globalPos, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// timer start
	if (id == 0)
	{
		ta = MPI_Wtime();
	}

	localPos = 0;
	for (i = 0; i < part; i++)
	{
		if (B[i] > B[localPos])
		{
			localPos = i;
		}
	}
	globalPos[0] = (id * part) + localPos;

	MPI_Gather(globalPos, 1, MPI_INT, MAX, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (id == 0)
	{		
		maxPos = MAX[0];
		for (i = 0; i < P; i++)
		{
			if (A[MAX[i]] > A[maxPos])
			{
				maxPos = MAX[i];
			}
		}

		tt = MPI_Wtime();
		// display array
		printf("\nArray A: \n");
		printf("\n");
		for(i = 0; i < N; i++)
		{
			printf("%d\t|", A[i]);
		}
		printf("\n");
		// print search results
		printf("\nLocal max positions: ");
		for (i = 0; i < P; i++)
		{
			printf("%d ", MAX[i]);
		}
		printf("\n");
		printf("\nMaximum value in array: %d\n", A[maxPos]);
		printf("\nMaximum value position in array: %d\n", maxPos);
		printf("\nTotal run time: %.6fs\n", tt - ta);
	}
	MPI_Finalize();
	return 0;
}