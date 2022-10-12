#include <iostream>
#include <ctime>

int main() {
    const int n = 30;
    int a[n];
    int count = 0;

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }
    for (int elem : a) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    #pragma omp parallel for num_threads(3)
    for (int i = 0; i < n; i++) {
        if (a[i] % 9 == 0) {
            #pragma omp atomic
            count += 1;
        }
    }
    printf("Number of multiples of 9 elements: %d\n", count);
}
