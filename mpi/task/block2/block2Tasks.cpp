#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>

int rank, size;

void print_matrix(double **m, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        std::cout << i << ": ";
        for (size_t j = 0; j < cols; ++j)
            std::cout << m[i][j] << '\t';
        std::cout << std::endl;
    }
}

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
            int rowStartNumber = (i - 1) * batch;

            if (n - rowStartNumber <= batch) {
                batch = n - rowStartNumber;
            }

            // отправляем сколько строк и с какого индекса нужно заполнить матрицу c
            MPI_Send(&batch, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(&rowStartNumber, 1, MPI_INT, i, 100, MPI_COMM_WORLD);

            for (int j = rowStartNumber; j < rowStartNumber + batch; j++) {
                MPI_Send(&a[j][0], n, MPI_DOUBLE, i, 101,MPI_COMM_WORLD);
                MPI_Send(&b[j][0], n, MPI_DOUBLE, i, 102,MPI_COMM_WORLD);
            }

            double batchC[n];
            MPI_Status statusC;
            for (int j = rowStartNumber; j < rowStartNumber + batch; j++) {
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
            int rowStartNumber = (i - 1) * batch;

            if (aRow - rowStartNumber <= batch) {
                batch = aRow - rowStartNumber;
            }

            // чтобы понять сколько раз должен быть send в потоках где rank != 0
            MPI_Send(&batch, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(&rowStartNumber, 1, MPI_INT, i, 100, MPI_COMM_WORLD);

            for (int j = rowStartNumber; j < rowStartNumber + batch; j++) {
                for (int k = 0; k < bColumn; k++) {
                    MPI_Send(&a[j][0], aColumn, MPI_DOUBLE, i, 101, MPI_COMM_WORLD);
                    MPI_Send(&transposedB[k][0], bRow, MPI_DOUBLE, i, 102, MPI_COMM_WORLD);
                }
            }

            for (int j = rowStartNumber; j < rowStartNumber + batch; j++) {
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

int task5() {
    const int aColumn = 2;
    const int aRow = 3;

    double a[aRow][aColumn];
    int row, column = 0;

    double b[aColumn][aRow];
    double v[aColumn];

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < aRow; i++) {
            for (int j = 0; j < aColumn; j++) {
                a[i][j] = rand() % 9;
            }
        }
        printf("Matrix A:\n");
        for (row = 0; row < aRow; row++) {
            for (column = 0; column < aColumn; column++) {
                printf("%f     ", a[row][column]);
            }
            printf("\n");
        }

        // сколько строк обработать для каждого потока
        int batch = ceil((double) aRow / (size - 1));

        for (int i = 1; i < size; i++) {
            // для каждого потока определяем номер первой строки из пачки
            int start = (i - 1) * batch;

            // для последнего потока размер пачки будет отличаться
            if (aRow - start <= batch) {
                batch = aRow - start;
            }

            // отправляем размер пачки и номер первой строки пачки
            MPI_Send(&batch, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Send(&start, 1, MPI_INT, i, 100, MPI_COMM_WORLD);

            // от номера первой строки пачки до последней (то есть отправляем целую пачку)
            for (int j = start; j < start + batch; j++) {
                MPI_Send(&a[j][0], aColumn, MPI_DOUBLE, i, 101,MPI_COMM_WORLD);
            }
        }

        for (int i = 1; i < size; i++) {
            MPI_Status statusC;
            MPI_Status status;
            MPI_Status statusCount;

            int start, count;
            double batchV[aColumn];

            MPI_Recv(&start, 1, MPI_INT, i, 102, MPI_COMM_WORLD, &status);
            MPI_Recv(&count, 1, MPI_INT, i, 103, MPI_COMM_WORLD,&statusCount);
            MPI_Recv(&batchV, aColumn, MPI_DOUBLE, i, 104, MPI_COMM_WORLD, &statusC);

            for (int j = start; j < start + count; j++) {
                for (int l = 0; l < aColumn; l++) {
                    b[l][j] = batchV[l];
                }
            }
        }
        printf("Matrix B:\n");
        for (row = 0; row < aColumn; row++) {
            for (column = 0; column < aRow; column++) {
                printf("%f     ", b[row][column]);
            }
            printf("\n");
        }
    }
    else {
        MPI_Status aStatus;
        MPI_Status elemStatus;
        MPI_Status startStatus;

        int count, start;

        MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &elemStatus);
        MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD,&startStatus);

        double batchA[aRow][aColumn];

        // заполняем то, что получили
        for (int i = start; i < start + count; i++) {
            MPI_Recv(&batchA[i][0], aColumn, MPI_DOUBLE, MPI_ANY_SOURCE, 101,MPI_COMM_WORLD, &aStatus);

            for (int j = 0; j < aColumn; j++) {
                v[j] = batchA[i][j];
            }
        }

        MPI_Send(&start, 1, MPI_INT, 0, 102, MPI_COMM_WORLD);
        MPI_Send(&count, 1, MPI_INT, 0, 103, MPI_COMM_WORLD);
        MPI_Send(&v, aColumn, MPI_DOUBLE, 0, 104, MPI_COMM_WORLD);
    }
    return 0;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    task4();
//    double starttime, endtime;
//    starttime = MPI_Wtime();
//    endtime = MPI_Wtime();
//    printf("Time:  %f s\n",endtime-starttime);
    MPI_Finalize();
    return 0;
}

