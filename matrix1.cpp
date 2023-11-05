#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>

int N, M, K;

// Function to initialize a matrix with random values
void initializeMatrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = rand() % 10; // Fill with random values between 0 and 9
    }
}

// Function to print a matrix
void printMatrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

// Function to multiply two matrices and store the result in a shared memory segment
void multiplyMatrices(int* A, int* B, int* C, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < K; ++j) {
            C[i * K + j] = 0;
            for (int k = 0; k < M; ++k) {
                C[i * K + j] += A[i * M + k] * B[k * K + j];
            }
        }
    }
}

int main() {
    srand(time(0));

    printf("Enter the number of rows for A matrix:");
    scanf("%d", &N);

    printf("Enter the number of columns for A matrix and row for matrix B:");
    scanf("%d", &M);

    printf("Enter the number of columns for B matrix:");
    scanf("%d", &K);

    // Calculate the size of the shared memory segment for matrix C
    int shm_size_C = N * K * sizeof(int);

    // Create shared memory for the resultant matrix C
    int shm_id_C = shmget(IPC_PRIVATE, shm_size_C, IPC_CREAT | 0666);

    if (shm_id_C == -1) {
        perror("shmget");
        exit(1);
    }

    int* C = (int*)shmat(shm_id_C, NULL, 0);

    // Initialize matrices with random values
    int A[N][M], B[M][K];

    initializeMatrix((int*)A, N, M);
    initializeMatrix((int*)B, M, K);

    printf("Matrix A:\n");
    printMatrix((int*)A, N, M);

    printf("\nMatrix B:\n");
    printMatrix((int*)B, M, K);

    // Define the number of child processes
    int numProcesses = N;

    clock_t start_time = clock(); // Record start time

    // Perform matrix multiplication using multiple child processes
    int childID = 0;
    pid_t parentPID = getpid();

    for (int i = 0; i < numProcesses; ++i) {
        if (getpid() == parentPID) {
            pid_t childPID = fork();
            if (childPID == 0) {
                // Child process computes a portion of the result matrix
                int startRow = i;
                int endRow = i + 1;
                multiplyMatrices((int*)A, (int*)B, C, startRow, endRow);
                exit(0);
            }
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < numProcesses; ++i) {
        wait(NULL);
    }

    clock_t end_time = clock(); // Record end time
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nResulting Matrix C:\n");
    printMatrix(C, N, K);

    printf("Execution Time: %.6f seconds\n", execution_time);

    // Detach and remove shared memory
    shmdt(C);
    shmctl(shm_id_C, IPC_RMID, NULL);

    return 0;
}
