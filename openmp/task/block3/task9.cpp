#include <iostream>
#include <ctime>

int main() {
    const int n = 6;
    const int m = 8;
    int min = INT_MAX;
    int max = INT_MIN;

    int d[n][m];

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            d[i][j] = rand() % 100;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << d[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    #pragma omp parallel for num_threads(3)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            #pragma omp critical
            if (d[i][j] > max) {
                max = d[i][j];
            }

            #pragma omp critical
            if (d[i][j] < min) {
                min = d[i][j];
            }
        }
    }

    printf("Array d: min = %d, max = %d", min, max);
}
