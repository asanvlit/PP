#include <iostream>
#include <ctime>

int main() {
    int a[10];
    int b[10];

    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
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

    int sumA = 0;
    int sumB = 0;

    #pragma omp parallel for reduction (+:sumA, sumB)
    for (int i = 0; i < 10; i++) {
        sumA += a[i];
        sumB += b[i];
    }

    if (sumA > sumB) {
        printf("The average of the array a <%f> is greater than average of the array b <%f>", sumA * 1.0 / 10, sumB * 1.0 / 10);
    } else if (sumA < sumB) {
        printf("The average of the array b <%f> is greater than average of the array a <%f>", sumB * 1.0 / 10, sumA * 1.0 / 10);
    } else if (sumA == sumB){
        printf("The average of the array a <%f> equals average of the array b <%f>", sumA * 1.0 / 10, sumA * 1.0 / 10);
    }
}
