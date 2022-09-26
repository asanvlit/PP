#include <iostream>
#include <ctime>
#include <omp.h>

int main() {
    const int kLength = 300;
    const int kMaxValue = 10;

    int m[kLength][kLength];
    int result[kLength];
    int v[kLength];

    srand(time(NULL));
    for (int i = 0; i < kLength; i++) {
        for (int j = 0; j < kLength; j++) {
            m[i][j] = rand() % kMaxValue;
        }
    }
    for (int i = 0; i < kLength; i++) {
        v[i] = rand() % kMaxValue;
    }

    double start = omp_get_wtime();
    for (int i = 0; i < kLength; i++) {
        int tmp = 0;
        for (int j = 0; j < kLength; j++){
            tmp += m[i][j] * v[j];
        }
        result[i] = tmp;
    }

    printf("Multiply the matrix by the vector sequentially:\nTime: %f\n", omp_get_wtime() - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 30)
    for (int i = 0; i < kLength; i++) {
        int tmp = 0;
        for (int j = 0; j < kLength; j++){
            tmp += m[i][j] * v[j];
        }
        result[i] = tmp;
    }
    printf("Multiply the matrix by the vector parallel (dynamic):\nTime: %f\n", omp_get_wtime() - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 30)
    for (int i = 0; i < kLength; i++) {
        int tmp = 0;
        for (int j = 0; j < kLength; j++){
            tmp += m[i][j] * v[j];
        }
        result[i] = tmp;
    }
    printf("Multiply the matrix by the vector parallel (guided):\nTime: %f\n", omp_get_wtime() - start);
}

