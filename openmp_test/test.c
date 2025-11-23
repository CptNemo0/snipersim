#include <omp.h>
#include <stdio.h>

int main() {
    #ifdef _OPENMP
    printf("OpenMP enabled\n");
    #else
    printf("OpenMP not enabled\n");
    #endif
    printf("Threads: %d\n", omp_get_max_threads());
    #pragma omp parallel
    {
        printf("Hello from thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
}

