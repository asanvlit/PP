#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>

int rank, size;

void task1() {
    const int n = 100;

    double x[n], y[n], z[n];
    int batch = n / (size - 1);

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 20;
            y[i] = rand() % 20;
        }

        printf("Vector x: \n");
        for (double elem : x) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
        printf("Vector y: \n");
        for (double elem : y) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;

        for (int j = 1; j < size; j++) {
            if (batch * (j + 1) > n) { // элементов в последнем batch может быть меньше, чем в других, если n % (size1-1) != 0
                MPI_Send(&x[batch * (j - 1)], n - (batch * (j - 1)), MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
                MPI_Send(&y[batch * (j - 1)], n - (batch * (j - 1)), MPI_DOUBLE, j, 100, MPI_COMM_WORLD);
            }
            else {
                MPI_Send(&x[batch * (j - 1)], batch, MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
                MPI_Send(&y[batch * (j - 1)], batch, MPI_DOUBLE, j, 100, MPI_COMM_WORLD);
            }
        }

        for (int i = 1; i < size; i++) {
            int count;

            MPI_Status status;
            MPI_Probe(MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &count);

            int processNum = status.MPI_SOURCE;
            double* batchZ = new double[count];
            MPI_Recv(batchZ, count, MPI_DOUBLE, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);

            for (int j = 0; j < count; j++) {
                z[j + (processNum - 1) * batch] = batchZ[j];
            }
        }

        printf("Vector z: \n");
        for (double elem : z) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
    else {
        int count;

        MPI_Status status;
        MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &count);
        double* batchX = new double[count];
        MPI_Recv(batchX, count, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &status);

        MPI_Probe(0, 100, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &count);
        double* batchY = new double[count];
        MPI_Recv(batchY, count, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD, &status);

        double * batchZ = new double[count];
        for (int i = 0; i < count; i++) {
            batchZ[i] = batchX[i] * batchY[i];
        }

        MPI_Send(batchZ, count, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);
        delete[] batchZ;
    }
}

void task2() {
    const int n = 100;

    double x[n], y[n];
    int batch = n / (size - 1);

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 20;
            y[i] = rand() % 20;
        }

        printf("Before: ----------------------------------\n");
        printf("Vector x: \n");
        for (double elem : x) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
        printf("Vector y: \n");
        for (double elem : y) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;

        for (int j = 1; j < size; j++) {
            if (batch * (j + 1) > n) { // элементов в последнем batch может быть меньше, чем в других, если n % (size1-1) != 0
                MPI_Send(&x[batch * (j - 1)], n - (batch * (j - 1)), MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
                MPI_Send(&y[batch * (j - 1)], n - (batch * (j - 1)), MPI_DOUBLE, j, 100, MPI_COMM_WORLD);
            }
            else {
                MPI_Send(&x[batch * (j - 1)], batch, MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
                MPI_Send(&y[batch * (j - 1)], batch, MPI_DOUBLE, j, 100, MPI_COMM_WORLD);
            }
        }

        for (int i = 1; i < size; i++) {
            int count;

            MPI_Status status;
            MPI_Probe(MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &count);

            int processNum = status.MPI_SOURCE;
            double* batchX = new double[count];
            MPI_Recv(batchX, count, MPI_DOUBLE, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
            double* batchY = new double[count];
            MPI_Recv(batchY, count, MPI_DOUBLE, MPI_ANY_SOURCE, 102, MPI_COMM_WORLD, &status);

            for (int j = 0; j < count; j++) {
                x[j + (processNum - 1) * batch] = batchX[j];
                y[j + (processNum - 1) * batch] = batchY[j];
            }
            delete[] batchX;
            delete[] batchY;
        }

        printf("After: ----------------------------------\n");
        printf("Vector x: \n");
        for (double elem : x) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
        printf("Vector y: \n");
        for (double elem : y) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
    else {
        int count;

        MPI_Status status;
        MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &count);
        double* batchX = new double[count];
        MPI_Recv(batchX, count, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &status);

        MPI_Probe(0, 100, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &count);
        double* batchY = new double[count];
        MPI_Recv(batchY, count, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            double temp = batchX[i];
            batchX[i] = batchY[i];
            batchY[i] = temp;
        }

        MPI_Send(batchX, count, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);
        MPI_Send(batchY, count, MPI_DOUBLE, 0, 102, MPI_COMM_WORLD);
    }
}

void task3() {
    const int n = 4;

    double a[n][n], b[n][n], c[n][n];

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = rand() % 20;
                b[i][j] = rand() % 20;
            }
        }

        int row, column = 0;
        printf("Matrix A:\n");
        for (row = 0; row < n; row++) {
            for (column = 0; column < n; column++) {
                printf("%f     ", a[row][column]);
            }
            printf("\n");
        }
        row, column = 0;
        printf("Matrix B:\n");
        for (row = 0; row < n; row++) {
            for (column = 0; column < n; column++) {
                printf("%f     ", b[row][column]);
            }
            printf("\n");
        }

        int batch = ceil((double) n / (size - 1));

        for (int i = 1; i < size; i++) {
            int sendBatch;
            int rowStartNumber = (i - 1) * batch;

            if (n - rowStartNumber <= batch) {
                sendBatch = n - rowStartNumber;
            } else {
                sendBatch = batch;
            }

            // отправляем сколько строк и с какого индекса нужно заполнить матрицу c
            MPI_Send(&sendBatch, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(&rowStartNumber, 1, MPI_INT, i, 100, MPI_COMM_WORLD);

            for (int j = rowStartNumber; j < rowStartNumber + sendBatch; j++) {
                MPI_Send(&a[j][0], n, MPI_DOUBLE, i, 101,MPI_COMM_WORLD);
                MPI_Send(&b[j][0], n, MPI_DOUBLE, i, 102,MPI_COMM_WORLD);
            }
        }

        for (int i = 1; i < size; i++) {
            int receivedBatchSize;
            int rowStartNumber = (i - 1) * batch;

            if (n - rowStartNumber <= batch) {
                receivedBatchSize = n - rowStartNumber;
            } else {
                receivedBatchSize = batch;
            }

            double batchC[n];
            MPI_Status statusC;
            for (int j = rowStartNumber; j < rowStartNumber + receivedBatchSize; j++) {
                MPI_Recv(batchC, n, MPI_DOUBLE, i, 103,MPI_COMM_WORLD, &statusC);

                for (int k = 0; k < n; k++) {
                    c[j][k] = batchC[k];
                }
            }
        }

        row, column = 0;
        printf("Matrix C:\n");
        for (row = 0; row < n; row++) {
            for (column = 0; column < n; column++) {
                printf("%f     ", c[row][column]);
            }
            printf("\n");
        }
    }
    else {
        int count, start;
        double batchA[n][n], batchB[n][n], batchC[n];

        MPI_Status statusA;
        MPI_Status statusB;
        MPI_Status statusElem;
        MPI_Status statusStart;

        MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, 99,MPI_COMM_WORLD, &statusElem);
        MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD,&statusStart);

        for (int i = start; i < start + count; i++) {
            MPI_Recv(&batchA[i], n, MPI_DOUBLE, MPI_ANY_SOURCE, 101,MPI_COMM_WORLD, &statusA);
            MPI_Recv(&batchB[i], n, MPI_DOUBLE, MPI_ANY_SOURCE, 102,MPI_COMM_WORLD, &statusB);

            for (int j = 0; j < n; j++) {
                batchC[j] = batchA[i][j] * batchB[i][j];
            }
            MPI_Send(&batchC, n, MPI_DOUBLE, 0, 103, MPI_COMM_WORLD);
        }
    }
}

int task4() {
    int aColumn = 2;
    int aRow = 3;
    int bColumn = 3;
    int bRow = 2;

    if (aColumn != bRow) {
        return -1;
    }

    double a[aRow][aColumn], b[bRow][bColumn], c[aRow][bColumn];

    int batch = ceil((double) aRow / (size - 1));

    if (rank == 0) {
//        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < aRow; i++) {
            for (int j = 0; j < aColumn; j++) {
                a[i][j] = rand() % 10;
            }
        }
        for (int i = 0; i < bRow; i++) {
            for (int j = 0; j < bColumn; j++) {
                b[i][j] = rand() % 10;
            }
        }

        printf("Matrix A:\n");
        for (int row = 0; row < aRow; row++) {
            for (int column = 0; column < aColumn; column++) {
                printf("%f     ", a[row][column]);
            }
            printf("\n");
        }
        printf("Matrix B:\n");
        for (int row = 0; row < bRow; row++) {
            for (int column = 0; column < bColumn; column++) {
                printf("%f     ", b[row][column]);
            }
            printf("\n");
        }

        double transposedB[bColumn][bRow];

        for (int i = 0; i < aRow; i++) {
            for (int j = 0; j < aColumn; j++) {
                transposedB[i][j] = b[j][i];
            }
        }

        for (int i = 1; i < size; i++) {
            int sendBatch;
            int rowStartNumber = (i - 1) * batch;

            if (aRow - rowStartNumber <= batch) {
                sendBatch = aRow - rowStartNumber;
            } else {
                sendBatch = batch;
            }

            // чтобы понять сколько раз должен быть send в потоках где rank != 0
            MPI_Send(&sendBatch, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(&rowStartNumber, 1, MPI_INT, i, 100, MPI_COMM_WORLD);

            for (int j = rowStartNumber; j < rowStartNumber + sendBatch; j++) {
                for (int k = 0; k < bColumn; k++) {
                    MPI_Send(&a[j][0], aColumn, MPI_DOUBLE, i, 101, MPI_COMM_WORLD);
                    MPI_Send(&transposedB[k][0], bRow, MPI_DOUBLE, i, 102, MPI_COMM_WORLD);
                }
            }
        }

        for (int i = 1; i < size; i++) {
            int receivedBatchSize;
            int rowStartNumber = (i - 1) * batch;

            if (aRow - rowStartNumber <= batch) {
                receivedBatchSize = aRow - rowStartNumber;
            } else {
                receivedBatchSize = batch;
            }

            for (int j = rowStartNumber; j < rowStartNumber + receivedBatchSize; j++) {
                for (int k = 0; k < bColumn; k++) {
                    double *batchC = new double[1];
                    MPI_Status status;

                    MPI_Recv(batchC, 1, MPI_DOUBLE, i, 103, MPI_COMM_WORLD, &status);
                    c[j][k] = batchC[0];
                }
            }
        }

        printf("Matrix C:\n");
        for (int row = 0; row < aRow; row++) {
            for (int column = 0; column < bColumn; column++) {
                printf("%f     ", c[row][column]);
            }
            printf("\n");
        }
    } else {
        int count, start;
        double a1[aRow], b1[bRow];
        double *batchC = new double[1];

        MPI_Status statusA;
        MPI_Status statusB;
        MPI_Status statusElem;
        MPI_Status statusStart;

        MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, 99,MPI_COMM_WORLD, &statusElem);
        MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD,&statusStart);

        for (int i = start; i < start + count; i++) {
            for (int k = 0; k < bColumn; k++) {
                batchC[0] = 0.0;

                MPI_Recv(&a1, aColumn, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD, &statusA);
                MPI_Recv(&b1, bRow, MPI_DOUBLE, 0, 102, MPI_COMM_WORLD, &statusB);

                for (int j = 0; j < aRow; j++) {
                    batchC[0] += a1[j] * b1[j];
                }

                MPI_Send(batchC, 1, MPI_DOUBLE, 0, 103, MPI_COMM_WORLD);
            }
        }
    }

    return 0;
}

int task5 () {
    int aRow = 5;
    int aColumn = 5;

    if ((size - 1) % 2 != 0) {
        return -1;
    }

    double a[aRow][aColumn];
    double aTransposed[aColumn][aRow];

    int batchRow, batchColumn;
    batchRow = ceil(aRow * 1.0 / 2);
    batchColumn = ceil((double) aColumn / ((size - 1) * 1.0 / 2));

    if (rank == 0) {
//        srand(static_cast<unsigned int>(time(0)));
        int tmp[size - 1][4];

        int counter = 1;
        for (int i = 0; i < aRow; i++) {
            for (int j = 0; j < aColumn; j++) {
                a[i][j] = counter++;
            }
        }
        printf("Matrix A:\n");
        int row, column = 0;
        for (row = 0; row < aRow; row++) {
            for (column = 0; column < aColumn; column++) {
                printf("%f     ", a[row][column]);
            }
            printf("\n");
        }

        for (int i = 1; i < size; i++) {
            int columnStartNumber, rowStartNumber, rowsCount, columnsCount;

            if (i <= size / 2) {
                columnStartNumber = (i - 1) * batchColumn;
                rowStartNumber = 0;
                rowsCount = batchRow;
            } else {
                columnStartNumber = (i - (size - 1) / 2 - 1) * batchColumn;
                rowStartNumber = ceil(aRow * 1.0 / 2);
                rowsCount = aRow - aRow / 2 - bool(aRow % 2 != 0);
            }
            if (aColumn - columnStartNumber < batchColumn) {
                columnsCount = aColumn - columnStartNumber;
            } else {
                columnsCount = batchColumn;
            }
            tmp[i - 1][0] = rowsCount;
            tmp[i - 1][1] = columnsCount;
            tmp[i - 1][2] = rowStartNumber;
            tmp[i - 1][3] = columnStartNumber;

            double aPart[rowsCount][columnsCount];
            for (int i1 = rowStartNumber, i2 = 0; i1 <= rowStartNumber + rowsCount - 1; i1++, i2++) {
                for (int j1 = columnStartNumber, j2 = 0; j1 <= columnStartNumber + columnsCount - 1; j1++, j2++) {
                    aPart[i2][j2] = a[i1][j1];
                }
            }

            MPI_Send(&rowsCount, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(&columnsCount, 1, MPI_INT, i, 100, MPI_COMM_WORLD);
            MPI_Send(&aPart[0][0], rowsCount * columnsCount, MPI_DOUBLE, i, 101, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++) {
            int columns = tmp[i-1][0];
            int rows = tmp[i-1][1];
            int columnNumberStart = tmp[i-1][2];
            int rowNumberStart = tmp[i-1][3];

            double aPartTransposed[rows][columns];

            MPI_Status status;
            MPI_Recv(*aPartTransposed, rows * columns, MPI_DOUBLE, i, 102, MPI_COMM_WORLD, &status);

            for (int j1 = rowNumberStart, j2 = 0; j1 <= rowNumberStart + rows - 1; j1++, j2++) {
                for (int k1 = columnNumberStart, k2 = 0; k1 <= columnNumberStart + columns - 1; k1++, k2++) {
                    aTransposed[j1][k1] = aPartTransposed [j2][k2];
                }
            }
        }

        printf("Transposed matrix :\n");
        for (int r = 0; r < aColumn; r++) {
            for (int c = 0; c < aRow; c++) {
                printf("%f     ", aTransposed[r][c]);
            }
            printf("\n");
        }
    } else {
        int rowsCount, columnsCount;

        MPI_Status statusRows;
        MPI_Status statusColumns;

        MPI_Recv(&rowsCount, 1, MPI_INT, MPI_ANY_SOURCE, 99,MPI_COMM_WORLD, &statusRows);
        MPI_Recv(&columnsCount, 1, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD,&statusColumns);

        double aPart[rowsCount][columnsCount];
        double aPartTransposed[columnsCount][rowsCount];

        MPI_Status statusPart;
        MPI_Recv(&aPart[0][0], rowsCount * columnsCount, MPI_DOUBLE, MPI_ANY_SOURCE, 101,MPI_COMM_WORLD, &statusPart);

        for (int i = 0; i < columnsCount; i++) {
            for (int j = 0; j < rowsCount; j++) {
                aPartTransposed[i][j] = aPart[j][i];
            }
        }
        MPI_Send(&aPartTransposed[0][0], columnsCount * rowsCount, MPI_DOUBLE, 0, 102, MPI_COMM_WORLD);
    }

    return 0;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    task3();
//    double starttime, endtime;
//    starttime = MPI_Wtime();
//    endtime = MPI_Wtime();
//    printf("Time:  %f s\n",endtime-starttime);
    MPI_Finalize();
    return 0;
}

