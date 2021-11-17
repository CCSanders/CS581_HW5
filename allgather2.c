/*
* Name: Colin Sanders
* Email: ccsanders6@crimson.ua.edu
* Course Section: CS581
* Homework #: 5 - Pairwise Exchange
*
* To Compile: mpicc -Wall -o allgather2 allgather_driver.c allgather2.c
* To Run: mpiexec -n NUM_PROCESSES ./allgather2
*/

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
#include <math.h>
#include <string.h>

int allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{

    int rank, size, i, phase, partner, partnerOffset;
    MPI_Aint lb, sizeofsendtype, sizeofrecvtype;
    void *bufptr, *recvptr;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    MPI_Type_get_extent(sendtype, &lb, &sizeofsendtype);
    MPI_Type_get_extent(recvtype, &lb, &sizeofrecvtype);

    int numIterations = (int)(log10(size) / log10(2));

    // fill recv buffer with own process's data (like a step 0)
    bufptr = recvbuf + (sizeofrecvtype * recvcount * rank);
    memcpy(bufptr, sendbuf, sizeofrecvtype * recvcount);

    for (phase = 0; phase < numIterations; i++)
    {
        partnerOffset = pow(2, phase);
        int bytesExchanged = partnerOffset * sizeofrecvtype * recvcount;

        //determine which neighbor to send to
        if (phase == 0)
        {
            if (rank % 2 == 0)
            {
                partner = rank + 1;
                recvptr = bufptr + bytesExchanged;
            }
            else
            {
                partner = rank - 1;
                recvptr = bufptr - bytesExchanged;
            }
        }
        else
        {
            if ((rank / partnerOffset) % 2 == 0)
            {
                partner = rank + partnerOffset;
                recvptr = bufptr + bytesExchanged;
            }
            else
            {
                partner = rank - partnerOffset;
                recvptr = bufptr - bytesExchanged;
            }
        }

        MPI_Sendrecv(bufptr, bytesExchanged, MPI_CHAR, partner, 0, recvptr, bytesExchanged, MPI_CHAR, rank, 0, comm, MPI_STATUS_IGNORE);
    }

    return 0;
}