/* Sample program to illustrate the use of nonblocking send and receive
   functions to implement a simple broadcast */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

	int i, rank, size, num=0;
	MPI_Request request[32];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		num = 12345;
		for (i = 1; i < size; i++) 
			MPI_Isend(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i-1]);
		MPI_Waitall(size-1, request, MPI_STATUSES_IGNORE);
	} else {
		/*
		MPI_Irecv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]);
		MPI_Wait(&request[0], MPI_STATUS_IGNORE);
		*/
    /* You can match a nonblocking send with a blocking receive */
		MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
	}

	printf("rank = %d, num = %d\n", rank, num);

	MPI_Finalize();
	return 0;
}