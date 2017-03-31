// MPI MinMax P2P
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int *A; // Array pointer
	int N; // Array size
	int i; // iterator
	int localMin, localMax; // min, max positions per node
	int posMin, posMax; // global min, max value positions

	int nodeID, nodeSUM; // MPI rank, size
	double ta, tt; // MPI timer start/stop

	// Initiate MPI
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
		ta = MPI_Wtime(); // timer start
	}

	localMin = nodeID;
	localMax = nodeID;
	for (i = nodeID; i < N; i += nodeSUM)
	{
		if (A[i] <= A[localMin])
			{
				localMin = i;
			}
			if (A[i] > A[localMax])
			{
				localMax = i;
			}	
	}

	if (nodeID != 0)
	{
		MPI_Send(&localMin, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
		MPI_Send(&localMax, 1, MPI_INT, 0, 20, MPI_COMM_WORLD);
	}

	if (nodeID == 0)
	{
		posMin = localMin;
		posMax = localMax;
		for (i = 1; i < nodeSUM; i++)
		{
			MPI_Recv(&localMin, 1, MPI_INT, i, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&localMax, 1, MPI_INT, i, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (A[localMin] < A[posMin])
			{
				posMin = localMin;
			}
			else if (A[localMax] > A[posMax])
			{
				posMax = localMax;
			}
		}

		tt = MPI_Wtime(); // timer stop
		
		printf("\nArray: \n");
		printf("\n");
		for(i = 0; i < N; i++)
		{
			printf("|%d| ", A[i]);
		}
		printf("\n");
		
		printf("\nMinimum value in array: %d\n", A[posMin]);
		printf("\nMaximum value in array: %d\n", A[posMax]);
		printf("\nTotal run time: %.6fs\n", tt - ta);
	}

	MPI_Finalize();
	return 0;
}
