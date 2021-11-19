/*
* Name: Colin Sanders
* Email: ccsanders6@crimson.ua.edu
* Course Section: CS581
* Homework #: 5 - Broadcast
*
* To Compile: mpicc -Wall -O3 -o allgather1 allgather_driver.c allgather1.c
* To Run: mpiexec -n NUM_PROCESSES ./allgather1
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Gather all data elements
int allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int rank, size, i, offset;
    MPI_Status *status;
    MPI_Request *request;
    MPI_Aint lb, sizeofsendtype, sizeofrecvtype;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    status = malloc(sizeof(MPI_Status) * (size + 1));
    request = malloc(sizeof(MPI_Request) * (size + 1));

    MPI_Type_get_extent(sendtype, &lb, &sizeofsendtype);
    MPI_Type_get_extent(recvtype, &lb, &sizeofrecvtype);

    //start by having every process send theirs to the root
    if (rank != 0)
    {
        //wait for root to gather all processes
        MPI_Isend(sendbuf, sizeofsendtype * sendcount, MPI_CHAR, 0, 0, comm, &request[0]);
        MPI_Wait(&request[0], MPI_STATUS_IGNORE);

        //wait for root to broadcast back full array
        MPI_Irecv(recvbuf, sizeofrecvtype * recvcount * size, MPI_CHAR, 0, 0, comm, &request[1]);
        MPI_Wait(&request[1], MPI_STATUS_IGNORE);
    }
    else
    {
        //P0 "sends" its array
        MPI_Isend(sendbuf, sizeofsendtype * sendcount, MPI_CHAR, 0, 0, comm, &request[0]);

        //Set up recv requests at the correct location in the recv array for each process
        for (i = 0; i < size; i++)
        {
            offset = sizeofrecvtype * recvcount * i;
            char *bufptr = recvbuf + offset;
            MPI_Irecv(bufptr, sizeofrecvtype * recvcount, MPI_CHAR, i, 0, comm, &request[i + 1]);
        }

        //Wait to complete the gather
        MPI_Waitall(size + 1, request, status);

        //Redistribute the full array to each process
        for (i = 1; i < size; i++)
        {
            MPI_Isend(recvbuf, sizeofsendtype * sendcount * size, MPI_CHAR, i, 0, comm, &request[i - 1]);
        }
        
        //Wait to complete the broadcast
        MPI_Waitall(size - 1, request, status);
    }

    free(request);
    free(status);
    return 0;
}