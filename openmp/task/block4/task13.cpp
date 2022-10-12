#include <iostream>
#include <valarray>

int main() {
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int decimalNumber = 0;

    #pragma omp parallel for reduction(+:decimalNumber)
    for (int i = 0; i < 30; i++) {
        if (a[29 - i] == 1) {
            decimalNumber += pow(2, i);
        }
    }
    printf("The decimal number is equal to %d", decimalNumber);
}

