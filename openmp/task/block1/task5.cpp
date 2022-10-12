#include <omp.h>
#include <iostream>
#include <ctime>

int main() {
    int d[6][8];

    srand(time(NULL));
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            d[i][j] = rand() % 100;
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << d[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    #pragma omp parallel sections num_threads(3)
    {
        #pragma omp section
        {
            int sum = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    sum += d[i][j];
                }
            }
            printf("Section 1. Average of the array: %d, thread: %d\n", sum / (6 * 8), omp_get_thread_num());
        }

        #pragma omp section
        {
            int min = d[0][0];
            int max = d[0][0];
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (d[i][j] > max) {
                        max = d[i][j];
                    }
                    if (d[i][j] < min) {
                        min = d[i][j];
                    }
                }
            }
            printf("Section 2. Min of the array: %d, Max of the array: %d, thread: %d\n",
                   min, max, omp_get_thread_num());
        }

        #pragma omp section
        {
            int count = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (d[i][j] % 3 == 0) {
                        count++;
                    }
                }
            }
            printf("Section 3. The number of elements that are multiples of three: %d, thread: %d\n",
                   count, omp_get_thread_num());
        }
    }
}

