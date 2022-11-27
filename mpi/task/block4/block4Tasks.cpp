#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <cmath>

int rank, size;

void task1() {
    // оцениваются 2/3 задач
}

void task2() {
    const int n = 12;

    if (rank == 0) {
        int x[n];

        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 10;
        }

        printf("Array x: \n");
        for (int elem : x) {
            std::cout << elem << ", ";
        }
        printf("\n");

        int blockSize = ceil(n * 1.0 / size) + 1;

        printf("Rank: <%d>, block: ", rank);
        for (int i = 0; i < blockSize; i++) {
            std::cout << x[i] << " ";
        }
        printf("\n");

        for (int j = 1; j < size; j++) {
            if ((blockSize - 1) * (j + 1) + 1 >= 12) {
                int count = 12 - ((blockSize - 1) * j + 1);
                if (count <= 0) {
                    count = 0;
                }
                MPI_Send(&x[(blockSize - 1) * j + 1], count, MPI_INT, j, 99, MPI_COMM_WORLD);
            } else {
                MPI_Send(&x[(blockSize - 1) * j + 1], blockSize - 1, MPI_INT, j, 99, MPI_COMM_WORLD);
            }
        }
    }
    else {
        int count;

        MPI_Status status;
        MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);

        int *block = new int[count];
        MPI_Recv(block, count, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);

        printf("Rank: <%d>, block: ", rank);
        if (count > 0) {
            for (int i = 0; i < count; i++) {
                std::cout << block[i] << " ";
            }
        } else {
            printf("<empty>");
        }
        printf("\n");
    }
}

void task3PingPong() {
    const int n = 500;
    const int messageCount = 5;

    char* message = new char[n];

    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < n; i++) {
        message[i] = (char) rand();
    }

    if (rank == 0) {
        printf("Message size: %d byte\n", n);

        for (int i = 0; i < messageCount; i++) {
            MPI_Status status;

            double startTime = MPI_Wtime();
            MPI_Send(&message[0], n, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
            char receivedMessage[n];
            MPI_Recv(&receivedMessage, n, MPI_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            double endTime = MPI_Wtime();
            printf("#%d, time spent: %f\n", i, endTime - startTime);
        }
    }
    else {
        for (int i = 0; i < messageCount; i++) {
            MPI_Status status;

            char receivedMessage[n];
            MPI_Recv(&receivedMessage, n, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Send(&receivedMessage, n, MPI_CHAR, 0, 99, MPI_COMM_WORLD);
        }
    }
}

void task3PingPing() {
    const int n = 500;
    const int messageCount = 5;

    char message[n];

    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < n; i++) {
        message[i] = (char) rand();
    }

    MPI_Request request;
    MPI_Status status;

    if (rank == 0) {
        printf("Message size: %d byte\n", n);
        for (int i = 0; i < messageCount; ++i) {
            double startTime = MPI_Wtime();
            MPI_Isend(message, n, MPI_CHAR, 1, 99, MPI_COMM_WORLD, &request);
            char receivedMessage[n];
            MPI_Irecv(&receivedMessage, n, MPI_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            double endTime = MPI_Wtime();
            printf("#%d, time spent: %f\n", i, endTime - startTime);
        }
    } else {
        for (int i = 0; i < messageCount; i++) {
            char receivedMessage[n];
            MPI_Irecv(receivedMessage, n, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Isend(message, n, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    task3PingPong();
    MPI_Finalize();
    return 0;
}

