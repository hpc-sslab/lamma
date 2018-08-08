#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#define MAX_AROWS   20
#define MAX_ACOLS   1000
#define MAX_BCOLS   20

int main(int argc, char *argv[])
{
    double a[MAX_AROWS][MAX_ACOLS];
    double b[MAX_ACOLS][MAX_BCOLS];
    double c[MAX_AROWS][MAX_BCOLS];
    double buf[MAX_ACOLS];
    double ans[MAX_ACOLS];

    int myid, master, numprocs, ierr;
    int i, j, numsent, sender;
    int anstype, row, arows, acols, brows, bcols, crows, ccols;

    MPI_Comm world, workers;
    MPI_Group world_group, worker_group;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    world = MPI_COMM_WORLD;
    MPI_Comm_size(world, &numprocs);
    MPI_Comm_rank(world, &myid);
    printf("Process %d of %d is alive", myid, numprocs);
}
