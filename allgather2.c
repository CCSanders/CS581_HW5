/**
 * 
 *
 * 
 * 
 * 
 **/

//Pairing algorithm:
//In step n=0, evens pair with next odd
//In steps n>0:
//    neighborOffset = 2^i (distance to correct neighbor)
//    if (rank / neighborOffset % 2 == 0)
//        neighbor = rank + neighborOffset
//    else
//        neighbor = rank - neighborOffset

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int
recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
    
}