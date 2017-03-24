// Pi calculation on multiple nodes
// Author: Mitsakos, 2017

#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	double Pi = 3.141592653689793238482643;
	double mypi, pi, h, sum, x;
	double ta, tt;

	int i, n;
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);	

	while (1)
	{

		if (rank == 0)
		{	
			printf("\nEnter number of spaces (enter 0 to quit): ");
			scanf("%d", &n);
		}		
		
		ta = MPI_Wtime(); // timer start
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // master broadcasts n

		if (n == 0) // invalid number of spaces
		{
			break;
		}
		else
		{
			h = 1.0 / (double) n;
			sum = 0.0;

			for (i = rank + 1; i <= n; i += size)
			{
				x = h * ((double) i - 0.5);
				sum += (4.0 / (1 + x * x));
			}
			mypi = h * sum;

			MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
			tt = MPI_Wtime(); // timer stop

			if (rank == 0)
			{
				printf("\n\nPi calculated to %.16f", pi);
				printf("\n\nwith an error margin of %.16f\n\n", fabs(pi - Pi));
				printf("Total run time: %.16fs\n", tt - ta);
			}
		}
	}	
	MPI_Finalize();
	return 0;
}
