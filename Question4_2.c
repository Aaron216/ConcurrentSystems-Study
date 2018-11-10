#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double function(int ii);

int main(int argc, char *argv[])
{
    int nn = 1024;
    int thread_count = 8;

    double sum = 0.0;

    #pragma omp parallel for num_threads(thread_count) reduction(+: sum)
    for (int ii = 0; ii <= nn; ii++) {
        sum += function(ii);
    }

    printf("    Total sum = %.1f\n", sum);
    return EXIT_SUCCESS;
}

double function(int ii) {
    int start = ii*(ii+1)/2;
    int finish = start + ii;
    double output = 0.0;

    for (int jj = start; jj <= finish; jj++) {
        output += sin(jj);
    }

    return output;
}
