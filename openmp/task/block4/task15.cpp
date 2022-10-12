#include <iostream>
#include <cmath>

bool isPrimeNumber(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int leftBorder;
    int rightBorder;

    printf("Enter the value of the left border of the range :\n");
    scanf("%d", &leftBorder);
    printf("Enter the value of the right border of the range :\n");
    scanf("%d", &rightBorder);

    #pragma omp parallel for ordered schedule(dynamic) num_threads(4)
    for (int i = leftBorder; i <= rightBorder; i++) {
        if (isPrimeNumber(i)) {
            printf("Prime number:  %d\n", i);
        }
    }
}
