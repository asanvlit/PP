#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

int rank, size;

void task1() {
    printf("Hello, World!");
}

void task2() {
    printf("Process %d of %d\n", rank, size);
}

void task3() {
    if (rank == 0) {
        int a[10];

        srand(time(NULL));
        for (int i = 0; i < 10; i++) {
            a[i] = rand();
        }
        printf("Sent array: \n");
        for (int elem : a) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;

        MPI_Send(&a, 10, MPI_INT, 1, 99, MPI_COMM_WORLD);
    } else {
        int a[10];
        MPI_Status status;
        MPI_Recv(&a, 10, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);

        printf("Received array: \n");
        for (int elem : a) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
}

void task4() {
    if (rank == 0) {
        int a[10];

        srand(time(NULL));
        for (int i = 0; i < 10; i++) {
            a[i] = rand();
        }
        printf("Sent array: \n");
        for (int elem: a) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;

        for (int i = 1; i < size; i++) {
            MPI_Send(a, 10, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,&status);

        int count;
        MPI_Get_count(&status, MPI_INT, &count);

        int a[count];
        MPI_Recv(a, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
        printf("Received array on process %d of %d: \n", rank, size);
        for (int elem : a) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
}

void task15() {
    const int a = 5;
    const int b = 10;
    const int n = 100;

    int x[n], y[n], z[n];
    int batch = n / (size - 1);

    if (rank == 0) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 20;
            y[i] = rand() % 20;
        }

        printf("Vector x: \n");
        for (int elem : x) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
        printf("Vector y: \n");
        for (int elem : y) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;

        for (int j = 1; j < size; j++) {
            if (batch * (j + 1) > n) { // элементов в последнем batch может быть меньше, чем в других, если n % (size1-1) != 0
                MPI_Send(&x[batch * (j - 1)], n - (batch * (j - 1)), MPI_INT, j, 99, MPI_COMM_WORLD);
                MPI_Send(&y[batch * (j - 1)], n - (batch * (j - 1)), MPI_INT, j, 100, MPI_COMM_WORLD);
            }
            else {
                MPI_Send(&x[batch * (j - 1)], batch, MPI_INT, j, 99, MPI_COMM_WORLD);
                MPI_Send(&y[batch * (j - 1)], batch, MPI_INT, j, 100, MPI_COMM_WORLD);
            }
        }

        for (int i = 1; i < size; i++) {
            int count;

            MPI_Status status;
            MPI_Probe(MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);

            int processNum = status.MPI_SOURCE;
            int* batchZ = new int[count];
            MPI_Recv(batchZ, count, MPI_INT, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);

            for (int j = 0; j < count; j++) {
                z[j + (processNum - 1) * batch] = batchZ[j];
            }
        }

        printf("Vector z: \n");
        for (int elem : z) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
    else {
        int count;

        MPI_Status status;
        MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* batchX = new int[count];
        MPI_Recv(batchX, count, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);

        MPI_Probe(0, 100, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* batchY = new int[count];
        MPI_Recv(batchY, count, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);

        int* batchZ = new int[count];
        for (int i = 0; i < count; i++) {
            batchZ[i] = a * batchX[i] + b * batchY[i];
        }

        MPI_Send(batchZ, count, MPI_INT, 0, 101, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    task4();
    MPI_Finalize();
    return 0;
}

