#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "my_rand.h"

long long int n = 1024;
double a = 0.0;
double b = 10.0;
int thread_count = 32;

double Calc_vals(long long int n, double a, double b, int thread_count);
double integrand(double x);

int main(int argc, char *argv[]) {
    double sum_int_val = Calc_vals(n, a, b, thread_count);
    double mc_estimate = (b - a) / ((double)n) * sum_int_val;

    printf("MC Int Est: %e\n", mc_estimate);
    return 0;
}

double Calc_vals(long long int n, double a, double b, int thread_count) {
    double sum_int_val = 0;

    #pragma omp parallel num_threads(thread_count) \
    default(none) reduction(+: sum_int_val) \
    shared(n, a, b, thread_count)
    {
        unsigned seed = (unsigned)omp_get_wtime();
        long long int i;
        double x;

        #pragma omp for
        for(i = 0; i <= n; i++) {
            x = (b - a)*my_drand(&seed) + a;
            sum_int_val += integrand(x);
        }
    }

    return sum_int_val;
}

double integrand(double x) {
    return x;
}
