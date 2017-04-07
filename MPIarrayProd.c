// MPI array elemements product
// Author: Mitsakos, 2017

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	int *A, *B;
	int N, part;
	int *prod, *allprod;
	int i;
	
	int id, P;
	double ta, tt;
	
	// MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	
	if (id == 0)
	{
		printf("\nInput N, size of array: ");
		scanf("%d", &N);
		
		A = malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
		{
			A[i] = rand()%10+1;
		}
		
		part = N/P;
		
		allprod = malloc(sizeof(int));
		allprod[0] = 1;
		
	}
	
	MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);
	B = malloc(part * sizeof(int));
	MPI_Scatter(A, part, MPI_INT, B, part, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (id == 0)
	{
		ta = MPI_Wtime();
	}
	
	prod = malloc(sizeof(int));
	prod[0] = 1;
	for (i = 0; i < part; i++)
	{
		prod[0] = prod[0] * B[i];
	}
	
	MPI_Reduce(prod, allprod, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
	
	if (id == 0)
	{
		tt = MPI_Wtime();
		printf("\nArray:\n");
		printf("\n");
		for (i = 0; i < N; i++)
		{
			printf("%d\t|", A[i]);
			
		}
		printf("\n");
		printf("\nArray product = %d\n", allprod[0]);
		printf("\nTime = %.6f\n", tt - ta);
	}
	MPI_Finalize();
	return 0;
}
		
