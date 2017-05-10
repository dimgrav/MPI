// MPI Ring implementation
// Author: Dimitris Gravanis, 2017

#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	// initialize MPI
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int num; // number sent by master node to the next node

	if (rank == 0)
	{
		printf("Insert number to be sent from node %d: \n", rank);
		scanf("%d", &num);
		MPI_Send(&num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		// receives from the final node and displays result
		MPI_Recv(&num, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("The number after the final node %d is: %d\n", size - 1, num);
	}
	else
	{
		MPI_Recv(&num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		num += 1;

		if (rank == size - 1)
		{
			MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		else
		{	
			MPI_Send(&num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			printf("The number after node %d is: %d\n", rank, num);
		}
	}
	MPI_Finalize();
	return 0;
}
