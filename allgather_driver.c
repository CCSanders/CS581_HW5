/* Driver program to test the allgather function.
 *
 * To Compile: mpicc -Wall -o myallgather1 allgather_driver.c myallgather1.c
 *             mpicc -Wall -o myallgather2 allgather_driver.c myallgather2.c
 * To Run: On your local system: mpiexec -n 4 ./myallgather 
 *         On DMC cluster, see instructions provided in README.md
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

#define MAXN 262144 //number of ints to send, 4 bytes each
#define NTIMES 100

int allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
              void *recvbuf, int recvcount, MPI_Datatype recvtype,
              MPI_Comm comm);

int main(int argc, char **argv)
{
  int i, j, rank, size, msgsize, offset;
  int *sendbuf = NULL, *recvbuf = NULL;
  double t1, t2;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  sendbuf = (int *)malloc(sizeof(int) * MAXN); //each process has a buffer of size MAXN * 4 bytes
  for (i = 0; i < MAXN; i++)
    sendbuf[i] = rank * MAXN + i;

  recvbuf = (int *)malloc(sizeof(int) * MAXN * size);

  for (msgsize = 8; msgsize <= MAXN; msgsize = msgsize << 1)
  {
    /* setup a synchronization point */
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    /* include rest of the program here */
    for (i = 0; i < NTIMES; i++)
    {
      allgather(sendbuf, msgsize, MPI_INT, recvbuf, msgsize, MPI_INT, MPI_COMM_WORLD);
    }
        /* program end here */
        t2 = MPI_Wtime() - t1;

    MPI_Reduce(&t2, &t1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    /* check if everyone got the correct results */
    for (i = 0; i < size; i++)
      for (j = 0, offset = i * MAXN; j < msgsize; j++)
        assert(recvbuf[i * msgsize + j] == (offset + j));

    if (rank == 0)
    {
      printf("Message size = %ld bytes, Maximum time = %g\n",
             msgsize * sizeof(int), t1 / 100.0);
    }
  }

  MPI_Finalize();

  return 0;
}
