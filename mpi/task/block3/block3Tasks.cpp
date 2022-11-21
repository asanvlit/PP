#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

int rank, size;

void task1() {
    const int n = 102;

    int x[n];
    int batchSize = n / size;
    if (n % size != 0) {
        batchSize++;
    }

    int receivedBatchSize = batchSize;
    if (batchSize * (rank + 1) > n) {
        receivedBatchSize = n - rank * batchSize;
    }

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            x[i] = -10 + rand() % 30;
        }
        printf("Vector x: \n");
        for (double elem : x) {
            std::cout << elem << ", ";
        }
        printf("\n");
    }

    int receivedBatch[receivedBatchSize];
    MPI_Scatter(&x, batchSize, MPI_INT, &receivedBatch, batchSize,
                MPI_INT, 0, MPI_COMM_WORLD);
    int batchSum = 0;
    for (int i = 0; i < receivedBatchSize; i++) {
        batchSum += abs(receivedBatch[i]);
    }

    int vectorNorm;
    MPI_Reduce(&batchSum, &vectorNorm, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Vector norm: %d\n", vectorNorm);
    }
}

void task2() {
    const int n = 10;

    int x[n];
    int y[n];

    int batchSize = n / size;
    if (n % size != 0) {
        batchSize++;
    }

    int receivedBatchSize = batchSize;
    if (batchSize * (rank + 1) > n) {
        receivedBatchSize = n - rank * batchSize;
    }

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            x[i] = -5 + rand() % 20;
            y[i] = -5 + rand() % 20;
        }

        printf("Vector x: \n");
        for (int elem: x) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
        printf("Vector y: \n");
        for (int elem: y) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

    int xReceivedBatch[receivedBatchSize];
    int yReceivedBatch[receivedBatchSize];
    MPI_Scatter(&x, batchSize, MPI_INT, &xReceivedBatch, batchSize, MPI_INT, 0,
                MPI_COMM_WORLD);
    MPI_Scatter(&y, batchSize, MPI_INT, &yReceivedBatch, batchSize, MPI_INT, 0,
                MPI_COMM_WORLD);

    int batchSum = 0;
    for (int i = 0; i < receivedBatchSize; i++) {
        batchSum += xReceivedBatch[i] * yReceivedBatch[i];
    }

    int scalar;
    MPI_Reduce(&batchSum, &scalar, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Scalar product of vectors: %d\n", scalar);
    }
}

void task3() {
    const int n = 5;
    const int m = 5;

    int matrix[n][m];
    int v[m];

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i][j] = -5 + rand() % 20;
            }
        }

        for (int i = 0; i < m; i++) {
            v[i] = -5 + rand() % 20;
        }

        printf("Matrix A:\n");
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", matrix[row][column]);
            }
            printf("\n");
        }
        printf("Vector v: \n");
        for (int elem: v) {
            std::cout << elem << '\n';
        }
        std::cout << std::endl;
    }

    int batchSize = n / size;
    if (n % size != 0) {
        batchSize++;
    }

    int receivedBatchSize = batchSize;
    if (batchSize * (rank + 1) > n) {
        receivedBatchSize = n - rank * batchSize;
    }

    int receivedMatrix[receivedBatchSize][m];
    MPI_Scatter(&matrix, n * batchSize, MPI_INT, &receivedMatrix, n * batchSize,
                MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&v, m, MPI_INT, 0, MPI_COMM_WORLD);

    int batchVector[receivedBatchSize];
    for (int i = 0; i < receivedBatchSize; i++) {
        batchVector[i] = 0;
        for (int j = 0; j < n; j++) {
            batchVector[i] += receivedMatrix[i][j] * v[j];
        }
    }

    int result[n];
    MPI_Gather(batchVector, receivedBatchSize, MPI_INT, result,receivedBatchSize,
               MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result: \n");
        for (int elem: result) {
            std::cout << elem << '\n';
        }
        std::cout << std::endl;
    }
}

void task4() {
    const int m = 5;
    const int n = 5;

    int matrix[m][n];

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                matrix[i][j] = -5 + rand() % 20;
            }
        }

        printf("Matrix A:\n");
        for (int row = 0; row < m; row++) {
            for (int column = 0; column < m; column++) {
                printf("%d     ", matrix[row][column]);
            }
            printf("\n");
        }
    }

    int batchSize = m / size;
    if (m % size != 0) {
        batchSize++;
    }

    int receivedBatchSize = batchSize;
    if (batchSize * (rank + 1) > m) {
        receivedBatchSize = m - rank * batchSize;
    }

    int receivedMatrix[receivedBatchSize][n];
    MPI_Scatter(&matrix, n * batchSize, MPI_INT, &receivedMatrix, n * batchSize,
                MPI_INT, 0, MPI_COMM_WORLD);

    int batchVector[receivedBatchSize];
    for (int i = 0; i < receivedBatchSize; i++) {
        batchVector[i] = 0;
        for (int j = 0; j < n; ++j) {
            batchVector[i] += abs(receivedMatrix[i][j]);
        }
    }
    int maxVector = batchVector[0];
    for (int i = 1; i < receivedBatchSize; i++) {
        if (batchVector[i] > maxVector) {
            maxVector = batchVector[i];
        }
    }

    int result;
    MPI_Reduce(&maxVector, &result, 1, MPI_INT, MPI_MAX, 0,MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Result: %d\n", result);
    }
}

void task5() {
    const int n = 5;

    int a[n][n];
    int b[n][n];

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = -5 + rand() % 20;
                b[i][j] = -5 + rand() % 20;
            }
        }

        printf("Matrix A:\n");
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", a[row][column]);
            }
            printf("\n");
        }
        printf("Matrix B:\n");
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", b[row][column]);
            }
            printf("\n");
        }
    }

    int batchSize = n / size;
    if (n % size != 0) {
        batchSize++;
    }

    int receivedBatchSize = batchSize;
    if (batchSize * (rank + 1) > n) {
        receivedBatchSize = n - rank * batchSize;
    }

    int aReceived[batchSize][n];
    int bReceived[batchSize][n];
    MPI_Scatter(&a, n * batchSize, MPI_INT, &aReceived, n * batchSize,
                MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&b, n * batchSize, MPI_INT, &bReceived, n * batchSize,
                MPI_INT, 0, MPI_COMM_WORLD);

    int batchResult[receivedBatchSize][n];
    for (int i = 0; i < receivedBatchSize; ++i) {
        for (int j = 0; j < n; ++j) {
            batchResult[i][j] = aReceived[i][j] * bReceived[i][j];
        }
    }

    int c[n][n];
    MPI_Gather(batchResult, n * receivedBatchSize, MPI_INT, c,
               n * receivedBatchSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix C:\n");
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", c[row][column]);
            }
            printf("\n");
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    task5();
    MPI_Finalize();
    return 0;
}

