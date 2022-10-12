#include <iostream>

int main() {
    int number = 210;
    int square = 0;

    #pragma omp parallel for reduction(+:square)
    for (int i = 1; i < number * 2; i += 2) {
        square += i;
    }
    printf("The square of the number is equal to %d\n", square);
}