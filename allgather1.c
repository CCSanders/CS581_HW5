#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

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

    //start by having every process send theirs to the root
    if(rank != root){
        MPI_Type_get_extend(sendtype, &lb, &sizeofsendtype);
        offset = sizeofsendtype * sendcount * i;
    }
}