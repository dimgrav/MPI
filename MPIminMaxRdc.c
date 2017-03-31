// MPI Array MinMax Broadcast
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int *A; // Array
	int N; // Array size
	int localMin, localMax; // min, max per node
	int i; // iterator
	int Min, Max; // min, max values in array

	int nodeID, nodeSUM; // MPI rank, size
	double ta, tt; // MPI timer start, stop

	// Initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &nodeID);
	MPI_Comm_size(MPI_COMM_WORLD, &nodeSUM);

	if (nodeID == 0)
	{
		printf("\nInsert array size N: ");
		scanf("%d", &N);
	}

	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	A = (int *)malloc(N * sizeof(int));

	for (i = 0; i < N; i++)
	{
		A[i] = rand() % 10000;
	}

	if (nodeID == 0)
	{
		ta = MPI_Wtime();
	}

	localMin = nodeID;
	localMax = nodeID;
	for (i = nodeID; i < N; i += nodeSUM)
	{
		if (A[i] > A[localMax])
		{
			localMax = i;
		}
		else if (A[i] < A[localMin])
		{
			localMin = i;
		}
	}

	MPI_Reduce(&A[localMin], &Min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(&A[localMax], &Max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (nodeID == 0)
	{
		tt = MPI_Wtime();
		
		printf("\nArray: \n");
		printf("\n");
		for(i = 0; i < N; i++)
		{
			printf("|%d| ", A[i]);
		}
		printf("\n");
		
		printf("\nMinimum value in array: %d\n", Min);
		printf("\nMaximum value in array: %d\n", Max);
		printf("\nTotal run time: %.6fs\n", tt - ta);
	}

	MPI_Finalize();
	return 0;
}