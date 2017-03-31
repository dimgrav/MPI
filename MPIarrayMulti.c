// MPI array multiplication
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int **A, **B, **C; // array A, array B, C: Product array
	int i, j, k; // iterators
	int N, L, M; // array dimensions
	int s; // holds C values

	int me, P; // MPI rank, size
	double ta, tt; // MPI timer start/stop

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	if (me == 0)
	{
		printf("\nInsert array A dimensions: \n");
		scanf("%d%d", &N, &L);
		printf("\nInsert array B dimensions %d x: ", L);
		scanf("%d", &M);
	}

	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&L, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Allocate memory
	A = malloc(N * sizeof(int *));
	for (i = 0; i < N; i++)
	{
		A[i] = (int *)malloc(sizeof(int) * L);
	}
	
	B = malloc(L * sizeof(int *));
	for (i = 0; i < L; i++)
	{
		B[i] = (int *)malloc(sizeof(int) * M);
	}

	C = malloc(N * sizeof(int *));
	for (i = 0; i < N; i++)
	{
		C[i] = (int *)malloc(sizeof(int) * M);
	}

	// add values to input arrays
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < L; j++)
		{
			A[i][j] = rand() % 100;
		}
	}

	for (i = 0; i < L; i++)
	{
		for (j = 0; j < M; j++)
		{
			B[i][j] = rand() % 100;
		}
	}

	// timer start
	if (me == 0)
	{
		ta = MPI_Wtime();
	}

	// product array values
	for (i = me; i < N; i += P)
	{
		for (j = 0; j < M; j++)
		{
			s = 0;
			for (k = 0; k < L; k++)
			{
				s += A[i][k] * B[k][j];
			}
			C[i][j] = s;
		}
	}

	if (me != 0)
	{
		for (i = me; i < N; i += P)
		{
			MPI_Send(&C[i][0], M, MPI_INT, 0, i, MPI_COMM_WORLD);
		}
	}

	if (me == 0)
	{
		for (j = 1; j < P; j++)
		{
			for (i = j; i < N; i += P)
			{
				MPI_Recv(&C[i][0], M, MPI_INT, j, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		tt = MPI_Wtime(); // timer stop
		
		printf("\nProduct array C of A and B:\n");
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < M; j++)
			{
				printf("| %d\t", C[i][j]);
				if (j == M - 1)
				{
					printf("\n");
				}
			}
		}
		
		printf("\nTotal run time: %.12f\n", tt - ta);
	}

	MPI_Finalize();
	return 0;
}
