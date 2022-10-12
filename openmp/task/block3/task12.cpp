#include <iostream>
#include <omp.h>
#include <unistd.h>

void solution1() {
    #pragma omp parallel
    {
        int threadsCount = omp_get_num_threads();
        for (int i = threadsCount - 1; i >= 0; i--) {
            #pragma omp barrier
            {
                if (i == omp_get_thread_num()) {
                    printf("Hello world! From thread <%d> of %d\n", omp_get_thread_num(), threadsCount);
                }
            }
        }
    }
}

void solution2() {
    int maxThreadNum = 7;

    #pragma omp parallel num_threads(maxThreadNum + 1) shared(maxThreadNum)
    {
        while(true) {
            if (omp_get_thread_num() == maxThreadNum) {
                printf("Hello world! From thread [%d] of %d\n", omp_get_thread_num(),
                       omp_get_num_threads());
                maxThreadNum--;
                break;
            }
        }
    }
}

void solution3() {
    #pragma omp parallel num_threads(8)
    {
        int threadNumber = omp_get_thread_num();
        int threadsCount = omp_get_num_threads();

        sleep((threadsCount - threadNumber) * 10);

        printf("Hello world! From thread [%d] of %d\n", threadNumber, threadsCount);
    }
}

int main() {
//    solution1();
    solution2();
//    solution3();
}
