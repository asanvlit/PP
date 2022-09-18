#include <omp.h>
#include <iostream>

int main() {
    const int kThreadsCountMin = 1;
    int threadsCount = 3;

    #pragma omp parallel num_threads(threadsCount) if (threadsCount > kThreadsCountMin)
    {
        printf("Thread [%d] of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }

    threadsCount = 1;

    #pragma omp parallel num_threads(threadsCount) if (threadsCount > kThreadsCountMin)
    {
        printf("Thread [%d] of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
}

