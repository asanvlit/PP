#include <omp.h>
#include <iostream>

int main() {
    int a = 5;
    int b = 5;

    printf("Before the first parallel block: a = %d, b = %d\n", a, b);
    #pragma omp parallel private(a) firstprivate(b) num_threads(2)
    {
        int threadNumber = omp_get_thread_num();
        a += threadNumber;
        b += threadNumber;
        printf("Inside the first parallel block (Thread [%d]): a = %d, b = %d\n", threadNumber, a, b);
    }
    printf("After the first parallel block: a = %d, b = %d\n", a, b);

    printf("Before the second parallel block: a = %d, b = %d\n", a, b);
    #pragma omp parallel shared(a) private(b) num_threads(4)
    {
        int threadNumber = omp_get_thread_num();
        a -= threadNumber;
        b -= threadNumber;
        printf("Inside the second parallel block (Thread [%d]): a = %d, b = %d\n", threadNumber, a, b);
    }
    printf("After the second parallel block: a = %d, b = %d\n", a, b);
}

