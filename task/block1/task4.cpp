#include <omp.h>
#include <iostream>
#include <ctime>

int main() {
    int size = 10;
    int a[size];
    int b[size];

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }

    std::cout << "Array a: [" << ' ';
    for (int elem : a) {
        std::cout << elem << ' ';
    }
    printf("]\n");

    std::cout << "Array b: [" << ' ';
    for (int elem : b) {
        std::cout << elem << ' ';
    }
    printf("]\n");

    #pragma omp parallel num_threads(2)
    {
        #pragma omp master
        {
            int min = a[0];
            for (int i = 1; i < size; i++) {
                if (a[i] < min){
                    min = a[i];
                }
            }
            printf("Min of array a: min = %d, master thread: %d\n", min, omp_get_thread_num());
        }

        #pragma omp single
        {
            int max = b[0];
            for (int i = 1; i < size; i++) {
                if (b[i] > max){
                    max = b[i];
                }
            }
            printf("Max of array b: max = %d, thread: %d\n", max, omp_get_thread_num());
        }
    }
}

