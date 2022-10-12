#include <iostream>
#include <ctime>

int main() {
    const int n = 30;
    int max = INT_MIN;

    int a[n];

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }

    #pragma omp parallel for num_threads(3)
    for (int i = 0; i < n; i++) {
        if (a[i] % 7 == 0) {
            #pragma omp critical
            if (a[i] > max) {
                max = a[i];
            }
        }
    }
    printf("Maximum multiple of 7 element = %d\n", max);
}
