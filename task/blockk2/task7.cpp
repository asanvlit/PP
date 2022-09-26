#include <iostream>
#include <omp.h>
#include <ctime>

int main() {
    int a[12];
    int b[12];
    int c[12];

    srand(time(NULL));
    #pragma omp parallel for schedule(static, 4) num_threads(3)
    for (int i = 0; i < 12; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
        printf("a[%d] = %d, b[%d] = %d - from thread <%d> of %d\n", i, a[i], i, b[i], omp_get_thread_num(),
               omp_get_num_threads());
    }

    #pragma omp parallel for schedule(dynamic, 4) num_threads(4)
    for (int i = 0; i < 12; i++) {
        c[i] = a[i] + b[i];
        printf("c[%d] = a[%d] + b[%d] = %d + %d = %d - from thread <%d> of %d\n", i, i, i, a[i], b[i], c[i],
               omp_get_thread_num(), omp_get_num_threads());
    }
}
