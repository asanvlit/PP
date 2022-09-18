#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel num_threads(8)
    {
        int threadsCount = omp_get_num_threads();
        int threadNumber = omp_get_thread_num();
        printf("Hello world! From thread [%d] of %d\n", threadNumber, threadsCount);
    }
}

