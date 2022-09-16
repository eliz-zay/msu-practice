#include <mpi.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <exception>
#include <stdexcept>

double EPS = 0.001; // to check result matrix


using namespace std;

void printMatrix(double* matrix, int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
        printf("%7.4f ", matrix[i * size + j]);
    }
    printf("\n");
  }
}

void randInit(double* matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)  
            matrix[i * size + j] = rand() % 10;
    }
}

void parseCommand(int argc, char** argv, int& size) {
    if (argc != 2) {
        throw invalid_argument("1 argument required: matrix size");
    }

    size = atoi(argv[1]);
}

void initProcess(int &procNum, int &procRank) {
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    if (procRank == 0) {
        cout << "Number of processes: " << procNum << endl;
    }
}

void initMatrix(double* &A,double* &B,double* &C , int size, int procRank) {
    if (procRank == 0) {
        A = new double [size * size];
        B = new double [size * size];
        C = new double [size * size];

        randInit(A, size);
        randInit(B, size);
    }
}

void flip(double* &matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < i; ++j) {
            double temp = matrix[i * size + j];
            matrix[i * size + j] = matrix[j * size + i];
            matrix[j * size + i] = temp;
        }
    }
}

void multMPI(double* A, double* B, double* &C, int size, int procNum, int procRank) {
    MPI_Status Status;
    int procPartSize = size / procNum;
    int procPartElems = procPartSize * size;

    double* bufA = new double[procPartElems];
    double* bufB = new double[procPartElems];
    double* bufC = new double[procPartElems];

    if (procRank == 0) {
        // flip B to send columns, not continious rows (as for A)
        flip(B, size);
    }

    MPI_Scatter(A, procPartElems, MPI_DOUBLE, bufA, procPartElems, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, procPartElems, MPI_DOUBLE, bufB, procPartElems, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < procPartSize; ++i) {
        for (int j = 0; j < procPartSize; ++j) {
            double temp = 0.0;
            for (int k = 0; k < size; ++k) {
                temp += bufA[i * size + k] * bufB[j * size + k];
            }

            bufC[i * size + j + procPartSize * procRank] = temp;
        }
    }

    int next, prev, index;
    for (int p = 1; p < procNum; ++p) {
        next = procRank == procNum - 1 ? 0 : procRank + 1;
        prev = procRank == 0 ? procNum - 1 : procRank - 1;

        // send to next, recieve from previous
        MPI_Sendrecv_replace(bufB, procPartElems, MPI_DOUBLE, next, 0, prev, 0, MPI_COMM_WORLD, &Status);

        index = procRank >= p ? procRank - p : procNum + procRank - p;

        for (int i = 0; i < procPartSize; ++i) {
            for (int j = 0; j < procPartSize; ++j) {
                double temp = 0.;
                for (int k = 0; k < size; ++k) {
                    temp += bufA[i * size + k] * bufB[j * size + k];
                }

                bufC[i * size + j + procPartSize * index] = temp;
            }
        }
    }

    MPI_Gather(bufC, procPartElems, MPI_DOUBLE, C, procPartElems, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    delete []bufA;
    delete []bufB;
    delete []bufC;
}

void printInfo(double* A, double* B, double* C, int size, double time) {
    cout << endl << "Time: " << time << endl;

    if (size <= 10) {
        cout << endl << "A:" << endl;
        printMatrix(A, size);
        cout << endl << "B:" << endl;
        printMatrix(B, size);
        cout << endl << "C:" << endl;
        printMatrix(C, size);
    } else {
        cout << "Matrix size >10, too large to print matrixes..." << endl;
    }
}

void checkMult(double* A, double* B, double* C, int size) {
    bool res = true;
    double* R = new double [size * size];

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            double temp = 0.;
            for (int k = 0; k < size; ++k) {
                temp += A[i * size + k] * B[k * size + j];
            }

            R[i * size + j] = temp;
            if (fabs(R[i * size + j] - C[i * size + j]) > EPS) {
                res = false;
            }
        }
    }

    if (!res) {
        throw logic_error("Wrong result of computation!");
    }
}

int main(int argc, char* argv[]) {
    try {
        int procNum; 
        int procRank;
        int maxSize;
        double *A, *B, *C;
        double beg, end, time, maxTime;
        ofstream file("dataMPI.csv", ios::app);

        MPI_Init(&argc, &argv);  

        parseCommand(argc, argv, maxSize);
        initProcess(procNum, procRank);

        for (int size = 200; size <= maxSize; size += 200) {
            initMatrix(A, B, C, size, procRank);

            beg = MPI_Wtime();
            multMPI(A, B, C, size, procNum, procRank);
            time = MPI_Wtime() - beg;

            MPI_Reduce(&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

            if (procRank == 0) {
                file << procNum << "\t" << size << "\t" << maxTime << endl;
            }

            if (procRank == 0) {
                delete[] A; 
                delete[] B; 
                delete[] C; 
            }
        }

        MPI_Finalize();

        file.close();

    } catch (exception& exc) {
        cout << exc.what() << endl;
    }
}