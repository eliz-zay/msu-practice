#include <mpi.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <math.h>
#include <tgmath.h>

#define MASTER_TO_SLAVE_TAG 1


using namespace std;

void printMatrix(double* matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << matrix[i * n + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int* getProcCoordinates(int procRank, int procNum) {
    int blockSize = int(cbrt(procNum));
    int* coordinates = new int[3];

    coordinates[0] = (procRank / blockSize) % blockSize;
    coordinates[1] = procRank % blockSize;
    coordinates[2] = (procRank / int(pow(double(blockSize), 2))) % blockSize;

    return coordinates;
}

int coordinatesToOffset(int* coordinates, int n, int procNum) {
    int blockNum = int(cbrt(procNum));
    int blockSize = n / blockNum;
    return coordinates[0] * n * blockSize + coordinates[1] * blockSize;
}

int coordinatesToRank(int* coordinates, int size) {
    return size * (size * coordinates[2] + coordinates[0]) + coordinates[1];
}

int* setCoordinates(int x, int y, int z) {
    int* coordinates = new int [3];
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
    return coordinates;
}

double* matrixFromBin(char* fileName, int& n, int procRank, int procNum) {
    char type;
    int m;
    size_t tempN, tempM;
    double* block = NULL;
    int blockNum = int(cbrt(procNum));
    int* coordinates = getProcCoordinates(procRank, procNum);

    MPI_File file;
    MPI_Status status;
    MPI_File_open(MPI_COMM_WORLD, fileName, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    MPI_File_read(file, &type, 1, MPI_CHAR, &status);
    MPI_File_set_view(file, sizeof(char), MPI_LONG, MPI_LONG, "native", MPI_INFO_NULL);
    MPI_File_read(file, &tempN, 1, MPI_LONG, &status);
    MPI_File_set_view(file, sizeof(char) + sizeof(size_t), MPI_LONG, MPI_LONG, "native", MPI_INFO_NULL);
    MPI_File_read(file, &tempM, 1, MPI_LONG, &status);

    n = static_cast<int>(tempN);

    int blockSize = n / blockNum;
    block = new double [blockSize * blockSize];
    int offset = coordinatesToOffset(coordinates, n, procNum);

    MPI_File_set_view(file, sizeof(char) + 2 * sizeof(size_t) + offset * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    if (coordinates[2] == 0) {
        for (int i = 0; i < blockSize; ++i) {
            MPI_File_read(file, &block[i * blockSize], blockSize, MPI_DOUBLE, &status);
            MPI_File_seek(file, n - blockSize, MPI_SEEK_CUR);
        }
    }

    MPI_File_close(&file);

    return block;
}

void matrixToBin(char* fileName, double* C, int n, int procRank, int procNum) {
    size_t tempN = n;
    double* block = NULL;
    int blockNum = int(cbrt(procNum));
    int blockSize = n / blockNum;
    int* coordinates = getProcCoordinates(procRank, procNum);

    MPI_File file;
    MPI_Status status;
    MPI_File_open(MPI_COMM_WORLD, fileName, MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    MPI_File_write(file, "d", 1, MPI_CHAR, &status);
    MPI_File_set_view(file, sizeof(char), MPI_LONG, MPI_LONG, "native", MPI_INFO_NULL);
    MPI_File_write(file, &tempN, 1, MPI_LONG, &status);
    MPI_File_set_view(file, sizeof(char) + sizeof(size_t), MPI_LONG, MPI_LONG, "native", MPI_INFO_NULL);
    MPI_File_write(file, &tempN, 1, MPI_LONG, &status);

    int offset = coordinatesToOffset(coordinates, n, procNum);
    MPI_File_set_view(file, sizeof(char) + 2 * sizeof(size_t) + offset * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    if (coordinates[2] == 0) {
        for (int i = 0; i < blockSize; ++i) {
            MPI_File_write(file, &C[i * blockSize], blockSize, MPI_DOUBLE, &status);
            MPI_File_seek(file, n - blockSize, MPI_SEEK_CUR);
        }
    }

    MPI_File_close(&file);
}

int main(int argc, char* argv[]) {
    MPI_Status status;
    int procRank, procNum, n;
    double *A, *B, *C, *resultC;
    double mainTime, maxMainTime, inTime, outTime, ioTime, maxIoTime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    
    if (argc != 4) {
        cout << "3 parameters required!" << endl;
        return 0;
    }
    
    char* fileA = argv[1];
    char* fileB = argv[2];
    char* fileC = argv[3];

    inTime = MPI_Wtime();
    A = matrixFromBin(fileA, n, procRank, procNum);
    B = matrixFromBin(fileB, n, procRank, procNum);
    inTime = MPI_Wtime() - inTime;

    int blockNum = int(cbrt(procNum));
    int blockSize = n / blockNum;
    int* coordinates = getProcCoordinates(procRank, procNum);

    // if (coordinates[2] == 0) {
    //     cout << "RANK " << procRank << endl;
    //     printMatrix(A, blockSize);
    // }

    mainTime = MPI_Wtime();

    // A

    // send A_{ik} from process (i,k,0) to (i,k,k)
    if (coordinates[2] == 0 && coordinates[1] != coordinates[2]) {
        int* destCoordinates = setCoordinates(coordinates[0], coordinates[1], coordinates[1]);
        int destRank = coordinatesToRank(destCoordinates, blockNum);
        MPI_Send(A, blockSize * blockSize, MPI_DOUBLE, destRank, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD);
    }

    if (coordinates[1] && coordinates[1] == coordinates[2]) {
        int* srcCoordinates = setCoordinates(coordinates[0], coordinates[1], 0);
        int srcRank = coordinatesToRank(srcCoordinates, blockNum);
        MPI_Recv(A, blockSize * blockSize, MPI_DOUBLE, srcRank, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &status);
    }

    // bcast A_{ik} from (i,k,k) to all (i,j,k)
    MPI_Comm COMM_LAYER, COMM_LAYER_ROW;
    MPI_Comm_split(MPI_COMM_WORLD, coordinates[2], procRank, &COMM_LAYER);

    MPI_Comm_split(COMM_LAYER, coordinates[0], procRank, &COMM_LAYER_ROW);
    int* rowRootCoordinates = setCoordinates(coordinates[0], coordinates[2], 0);
    int rowRoot = coordinatesToRank(rowRootCoordinates, blockNum) % blockNum;
    MPI_Bcast(A, blockSize * blockSize, MPI_DOUBLE, rowRoot, COMM_LAYER_ROW);

    // B

    // send B_{ik} from process (k,j,0) to (k,j,k)
    if (coordinates[2] == 0 && coordinates[0] != coordinates[2]) {
        int* destCoordinates = setCoordinates(coordinates[0], coordinates[1], coordinates[0]);
        int destRank = coordinatesToRank(destCoordinates, blockNum);
        MPI_Send(B, blockSize * blockSize, MPI_DOUBLE, destRank, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD);
    }

    if (coordinates[2] && coordinates[0] == coordinates[2]) {
        int* srcCoordinates = setCoordinates(coordinates[0], coordinates[1], 0);
        int srcRank = coordinatesToRank(srcCoordinates, blockNum);
        MPI_Recv(B, blockSize * blockSize, MPI_DOUBLE, srcRank, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &status);
    }

    // bcast B_{ik} from process (k,j,k) to all (i,j,k)
    MPI_Comm COMM_LAYER_COLUMN;
    MPI_Comm_split(COMM_LAYER, coordinates[1], procRank, &COMM_LAYER_COLUMN);
    int columnRoot = coordinates[2];
    MPI_Bcast(B, blockSize * blockSize, MPI_DOUBLE, columnRoot, COMM_LAYER_COLUMN);

    C = new double [blockSize * blockSize];
    for (int i = 0; i < blockSize; ++i) {
        for (int j = 0; j < blockSize; ++j) {
            C[i * blockSize + j] = 0;
        }
    }

    for (int i = 0; i < blockSize; ++i) {  
        for (int k = 0; k < blockSize; ++k) {
            double elemA = A[i * blockSize + k];
            for (int j = 0; j < blockSize; ++j) {
                C[i * blockSize + j] += elemA * B[k * blockSize + j];
            }
        }
    }

    if (coordinates[2] == 0) {
        resultC = new double [blockSize * blockSize];
    }

    MPI_Comm COMM_VERTICAL;
    int ijOffset = coordinates[0] * blockSize + coordinates[1];
    MPI_Comm_split(MPI_COMM_WORLD, ijOffset, procRank, &COMM_VERTICAL);
    MPI_Reduce(C, resultC, blockSize * blockSize, MPI_DOUBLE, MPI_SUM, 0, COMM_VERTICAL);

    mainTime = MPI_Wtime() - mainTime;

    outTime = MPI_Wtime();
    matrixToBin(fileC, resultC, n, procRank, procNum);
    outTime = MPI_Wtime() - outTime;

    ioTime = inTime + outTime;

    MPI_Reduce(&mainTime, &maxMainTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&ioTime, &maxIoTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        // ofstream file("time.csv", ios::app);
        // file << procNum << "\t" << n << "\t" << maxMainTime << "\t" << maxIoTime << endl;
        // file.close();

        delete[] resultC;
    }

    MPI_Comm_free(&COMM_LAYER);
    MPI_Comm_free(&COMM_LAYER_ROW);
    MPI_Comm_free(&COMM_LAYER_COLUMN);
    MPI_Comm_free(&COMM_VERTICAL);
    MPI_Finalize();

    delete[] A;
    delete[] B;
    delete[] C;
}
