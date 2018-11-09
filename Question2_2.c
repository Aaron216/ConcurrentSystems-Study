#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int n = 16;

void printArray(double array[], int size);

int main(void) {
    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int local_n = n / comm_sz;

    double *local_a = malloc(local_n * sizeof(double));
    double *a = malloc(n * sizeof(double));

    int i;
    if (my_rank == 0) {
        srandom(my_rank);
        for (i = 0; i < n; i++) {
//            a[i] = random() % 10;
            a[i] = 1.0;
        }
        printf("a[] = ");
        printArray(a, n);
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD); /* Barrier 1 */

    printf("1. Proc: %d > local_a[] = ", my_rank);
    printArray(local_a, local_n);

    int local_i;
    for (local_i = 0; local_i < local_n; local_i++) {
        local_a[local_i] = local_a[local_i] * local_a[local_i];
    }

    MPI_Barrier(MPI_COMM_WORLD); /* Barrier 2 */

    printf("2. Proc: %d > local_a[] = ", my_rank);
    printArray(local_a, local_n);

    double local_sum = 0.0;
    for (local_i = 0; local_i < local_n; local_i++) {
        local_sum = local_sum + local_a[local_i];
    }
    double total_sum = 0.0;
    MPI_Allreduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    double total_sqrt_sum = sqrt(total_sum);

    MPI_Barrier(MPI_COMM_WORLD); /* Barrier 3 */

    printf("3. Proc: %d > Total sqrt sum: %f\n", my_rank, total_sqrt_sum);

    for (local_i = 0; local_i < local_n; local_i++) {
        local_a[local_i] = sqrt(local_a[local_i]) / total_sqrt_sum;
    }
    MPI_Allgather(local_a, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); /* Barrier 4 */

    printf("4. Proc: %d > local_a[] = ", my_rank);
    printArray(a, n);

    MPI_Finalize();
    return 0;
}

void printArray(double array[], int size) {
    printf("[%.2f", array[0]);
    for (int ii = 1; ii < size; ii++) {
        printf(" %.2f", array[ii]);
    }
    printf("]\n");
}