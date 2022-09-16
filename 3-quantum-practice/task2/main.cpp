#include <mpi.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <complex>
#include <assert.h>
#include <cmath>
#include "time.h"
#include "sys/time.h"

#define EPS 1e-6

using namespace std;

typedef complex<double> complexd;

complexd* fromFile(char* inFile, int rank, unsigned long long procSize) {
    double elemBuffer[2];
    MPI_File file;

    complexd* v = new complexd [procSize];

    MPI_File_open(MPI_COMM_WORLD, inFile, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, 2 * procSize * rank * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    for (int i = 0; i < procSize; i++) {
        MPI_File_read(file, &elemBuffer, 2, MPI_DOUBLE, MPI_STATUS_IGNORE);
        v[i].real(elemBuffer[0]);
        v[i].imag(elemBuffer[1]);
    }

    MPI_File_close(&file);

    return v;
}

void toFile(char* outFile, complexd* B, unsigned n, int rank, int size, unsigned long long procSize) {
    double elemBuffer[2];
    MPI_File file;

    MPI_File_open(MPI_COMM_WORLD, outFile, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, 2 * procSize * rank * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    for (int i = 0; i < procSize; i++) {
        elemBuffer[0] = B[i].real();
        elemBuffer[1] = B[i].imag();
        MPI_File_write(file, &elemBuffer, 2, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }

    MPI_File_close(&file);
}

complexd* getRandomVector(unsigned long long procSize, int rank, int size) { 
    double procModule, module = 0;
    MPI_Status status;

    complexd *procVec = new complexd[procSize];
    unsigned int seed = time(NULL) + rank;

    for (long long unsigned i = 0; i < procSize; i++){
        procVec[i].real(rand_r(&seed) % 100 + 1);
        procVec[i].imag(rand_r(&seed) % 100 + 1);
        procModule += abs(procVec[i] * procVec[i]);
    }
    
    MPI_Reduce(&procModule, &module, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    module = sqrt(module);
    MPI_Bcast(&module, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (long long unsigned i = 0; i < procSize; i++) {
        procVec[i] /= module;
    }

    return procVec;
}

void OneQubitEvolution(complexd* in, complexd* out, complexd U[2][2], unsigned n, unsigned q, int rank, unsigned long long procSize) {
    int firstIdx = rank * procSize;
    int shift = n - q;
    int pow = 1 << shift;
    int rankChange = firstIdx ^ pow; // меняем на противоположный q-й бит
    rankChange /= procSize;

    if (rank != rankChange) {
        complexd* inNew = new complexd [procSize];
        MPI_Sendrecv(in, procSize, MPI_DOUBLE_COMPLEX, rankChange, 0, inNew, procSize, MPI_DOUBLE_COMPLEX, rankChange, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (rank > rankChange) { 
            for (int i = 0; i < procSize; i++) {
                out[i] = U[1][0] * inNew[i] + U[1][1] * in[i];
            }
        } else {
            for (int i = 0; i < procSize; i++) {
                out[i] = U[0][0] * in[i] + U[0][1] * inNew[i];
            }
        }

        delete[] inNew;
    } else {
        for (int i = 0; i < procSize; i++) {
            int i0 = i & ~pow;
            int i1 = i | pow;
            int iq = (i & pow) >> shift;
            out[i] = U[iq][0] * in[i0] + U[iq][1] * in[i1];
        }
    }
}

void parseArguments(int argc, char** argv, unsigned& q, unsigned& n, bool& readMode, bool& testMode, char*& readFile) {
    q = atoi(argv[1]);
    n = atoi(argv[2]);
    readMode = string(argv[3]).compare("read") == 0;
    testMode = string(argv[4]).compare("test") == 0;
    if (argc == 6) {
        readFile = argv[5];
    }
}

int main(int argc, char** argv) {
    bool readMode = false, testMode = false;
    int rank, size;
    unsigned q, n;
    complexd *procIn, *procOut;
    struct timeval start, stop;

    char* initVectorFile = "init_vector.bin";
    char* resultFile = "result_vector.bin";
    char* readFile;

    complexd U[2][2];
    complexd Ucoeff(1 / sqrt(2));
    U[0][0] = Ucoeff;
    U[0][1] = Ucoeff;
    U[1][0] = Ucoeff;
    U[1][1] = -Ucoeff;

    parseArguments(argc, argv, q, n, readMode, testMode, readFile);
    if (argc == 6) {
        initVectorFile = readFile;
    }
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned long long length = 1LLU << n;
    unsigned long long procSize = length / size;

    if (readMode) {
        procIn = fromFile(initVectorFile, rank, procSize);
    } else {
        procIn = getRandomVector(procSize, rank, size);
        toFile(initVectorFile, procIn, n, rank, size, procSize);
    }

    procOut = new complexd[procSize];

    double begin = MPI_Wtime();
    OneQubitEvolution(procIn, procOut, U, n, q, rank, procSize);
    double end = MPI_Wtime();

    if (testMode) {
        bool* allErrors = new bool [size];
        bool procError = false, error = false;
        complexd* testVector = fromFile(resultFile, rank, procSize);

        for (int i = 0; i < procSize; i++) {
            if (abs(testVector[i].real() - procOut[i].real()) > EPS || abs(testVector[i].imag() - procOut[i].imag()) > EPS) {
                procError = true;
                break;
            }
        }

        MPI_Gather(&procError, 1, MPI_C_BOOL, allErrors, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                if (allErrors[i]) {
                    error = true;
                    break;
                }
            }

            cout << (error ? "Error!" : "Correct") << endl;
        }

        delete[] allErrors;
    } else {
        toFile(resultFile, procOut, n, rank, size, procSize);
    }

    if (rank == 0) {
        ofstream file("time.csv", ios::app);
        file << q << " " << n << " " << size << " " << end - begin << endl;
        file.close();
    }

    MPI_Finalize();
    
    delete[] procOut;
    delete[] procIn;
}