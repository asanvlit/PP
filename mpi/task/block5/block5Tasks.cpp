#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <cmath>

int rank, size;

void task1() {
    const int n = 8;

    MPI_Datatype vector;
    MPI_Type_vector(1, n, n, MPI_INT, &vector);
    MPI_Type_commit(&vector);

    if (rank == 0) {
        int a[n][n];

        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = -5 + rand() % 20;
            }
        }
        printf("Matrix A:\n");
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", a[row][column]);
            }
            printf("\n");
        }

        for (int i = 0; i < n; i++) {
            MPI_Send(&a[i], 1, vector, ((i % 2) + 1), 99, MPI_COMM_WORLD);
        }

    } else if (rank == 1) {
        int c[n / 2][n];

        MPI_Status status;
        for (int i = 0; i < n / 2; i++) {
            MPI_Recv(&c[i], 1, vector, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        printf("Matrix C:\n");
        for (int row = 0; row < n / 2; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", c[row][column]);
            }
            printf("\n");
        }

        bool isPrinted = true;
        MPI_Send(&isPrinted, 1, MPI_C_BOOL, 2, 100, MPI_COMM_WORLD);
    } else if (rank == 2) {
        int b[n / 2][n];

        MPI_Status status;
        for (int i = 0; i < n / 2; i++) {
            MPI_Recv(&b[i], 1, vector, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        bool isAbleToPrint;
        MPI_Recv(&isAbleToPrint, 1, MPI_C_BOOL, 1, 100, MPI_COMM_WORLD, &status);

        printf("Matrix B:\n");
        for (int row = 0; row < n / 2; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", b[row][column]);
            }
            printf("\n");
        }
    }
}

void task2() {
    const int n = 8;
    const int blockSizes[2] = {n, n};
    const MPI_Datatype blockDataTypes[2] = {MPI_INT, MPI_INT};

    int off[4][n];

    MPI_Aint address[2];

    MPI_Get_address(&off[0][0], &address[0]);
    MPI_Get_address(&(off[4][0]), &address[1]);

    MPI_Aint offset[2] = {0, (address[1] - address[0])};

    MPI_Datatype MPI_ROWS_PAIR;
    MPI_Type_create_struct(2, blockSizes, offset, blockDataTypes, &MPI_ROWS_PAIR);
    MPI_Type_commit(&MPI_ROWS_PAIR);

    if (rank == 4) {
        int a[n][n];

        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = -5 + rand() % 20;
            }
        }
        printf("\nMatrix A:\n");
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < n; column++) {
                printf("%d     ", a[row][column]);
            }
            printf("\n");
        }

        for (int i = 0; i < 4; i++) {
            MPI_Send(&a[i], 1, MPI_ROWS_PAIR, i, 99, MPI_COMM_WORLD);
        }

        bool isPrinted = true;
        MPI_Send(&isPrinted, 1, MPI_C_BOOL, 1, 100, MPI_COMM_WORLD);
    } else {
        int rowsPair[5][n];

        MPI_Status status;
        MPI_Recv(&rowsPair, 1, MPI_ROWS_PAIR, 4, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int b[2][n];
        std::copy(rowsPair[0], std::end(rowsPair[0]), b[0]);
        std::copy(rowsPair[4], std::end(rowsPair[4]), b[1]);

        bool isAbleToPrint;
        MPI_Recv(&isAbleToPrint, 1, MPI_C_BOOL, (rank - 1), 100, MPI_COMM_WORLD, &status);

        printf("\n#%d, matrix rows pair:\n", rank);
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
        if (rank != (size - 1)) {
            MPI_Send(&isAbleToPrint, 1, MPI_C_BOOL, (rank + 1), 100, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    task2();
    MPI_Finalize();
    return 0;
}

