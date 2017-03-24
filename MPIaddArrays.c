// Array math operations
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	int *A, *B, *C, *D; // Array declarations
	int N; // length of arrays
	int part; // array part length per node, remainder after division
	int i; // iterator
	int me, P; // rank, size
	double ta, tt; // timer start/stop

	// MPI initialization
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	if (me == 0)
	{
		printf("\nInsert array length N: ");
		scanf("%d", &N);
		part = N / (P - 1);
		ta = MPI_Wtime();
	}
	
	MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
	A = malloc(N * sizeof(int));
	B = malloc(N * sizeof(int));
	C = malloc(N * sizeof(int));
	D = malloc(N * sizeof(int));

	for (i = 0; i < N; i++)
	{
		A[i] = rand() % 1000;
		B[i] = rand() % 1000;
	}	

	if (me != 0)
	{
		for (i = (me - 1) * part; i <= (me * part - 1); i++)
		{
			C[i] = A[i] + B[i];
			D[i] = A[i] - B[i];
		}
		MPI_Send(&C[(me - 1)*part], part, MPI_INT, 0, 10, MPI_COMM_WORLD);
		MPI_Send(&D[(me - 1)*part], part, MPI_INT, 0, 20, MPI_COMM_WORLD);
	}
		
	if (me == 0)
	{
		for (i = 1; i < P; i++)
		{
			MPI_Recv(&C[(i - 1)*part], part, MPI_INT, i, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&D[(i - 1)*part], part, MPI_INT, i, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		
		tt = MPI_Wtime();
		printf("\n\nTotal run time: %.16fs\n", tt - ta);
	}
	
	MPI_Finalize();
	return 0;
}