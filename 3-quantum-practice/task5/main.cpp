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

#include "qubitEvolution.hpp"

#define EPS 1e-6

using namespace std;

typedef complex<double> complexd;

void print(complexd* v, unsigned long long procSize) {
    for (unsigned long long i = 0; i < procSize; i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;;
}

void parseArguments(int argc, char** argv, unsigned& n, bool& readMode, bool& testMode, bool& createTestMode, char*& readFile, char*& testFile) {
    n = atoi(argv[1]);
    readMode = string(argv[2]).compare("read") == 0;
    testMode = string(argv[3]).compare("test") == 0;
    createTestMode = string(argv[4]).compare("create_test") == 0;
    if (argc == 7) {
        readFile = argv[5];
        testFile = argv[6];
    }
}

complexd* fromFile(char* inFile, int rank, unsigned long long procSize) {
    double elemBuffer[2];
    MPI_File file;

    complexd* v = new complexd [procSize];

    MPI_File_open(MPI_COMM_WORLD, inFile, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, 2 * procSize * rank * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    for (int i = 0; i < procSize; i++) {
        MPI_File_read(file, &elemBuffer, 2, MPI_DOUBLE, MPI_STATUS_IGNORE);
        v[i] = complexd(elemBuffer[0], elemBuffer[1]);
    }

    MPI_File_close(&file);

    return v;
}

void toFile(char* outFile, complexd* v, unsigned long long n, int rank, unsigned long long procSize) {
    double elemBuffer[2];
    MPI_File file;

    MPI_File_open(MPI_COMM_WORLD, outFile, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, 2 * procSize * rank * sizeof(double), MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);

    for (int i = 0; i < procSize; i++) {
        elemBuffer[0] = v[i].real();
        elemBuffer[1] = v[i].imag();
        MPI_File_write(file, &elemBuffer, 2, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }

    MPI_File_close(&file);
}

complexd* getRandomVector(unsigned long long procSize, int rank) { 
    double procModule = 0, module = 0;
    MPI_Status status;

    complexd* procVec = new complexd[procSize];
    unsigned int seed = time(NULL) + rank;

    for (long long unsigned i = 0; i < procSize; i++) {
        procVec[i] = complexd(rand_r(&seed) % 100 + 1, rand_r(&seed) % 100 + 1);
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

int main(int argc, char** argv) {
    bool readMode = false, testMode = false, createTestMode = false;
    int rank, procNum;
    unsigned n;
    complexd *in, *vec;
    struct timeval start, stop;

    char* initVectorFile = "init_vector.bin";
    char* resultFile = "result_vector.bin";
    char* readFile, *testFile;

    complexd H[2][2];
    complexd Hcoeff(1 / sqrt(2));
    H[0][0] = Hcoeff;
    H[0][1] = Hcoeff;
    H[1][0] = Hcoeff;
    H[1][1] = -Hcoeff;

    complexd R[4][4];
    R[0][0] = complexd(1, 0);
    R[1][1] = complexd(1, 0);
    R[2][2] = complexd(1, 0);

    parseArguments(argc, argv, n, readMode, testMode, createTestMode, readFile, testFile);
    if (argc == 7) {
        initVectorFile = readFile;
        resultFile = testFile;
    }

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);

    if (createTestMode && procNum != 1) {
        if (rank == 0) {
            cout << endl << "Error: tests creation requires 1 process." << endl;
            cout << "Enter 1 process with create_test flag." << endl << endl;
        }
        MPI_Finalize();
        exit(0);
    }

    unsigned long long length = 1LLU << n;
    unsigned long long procSize = length / procNum;

    if (readMode) {
        vec = fromFile(initVectorFile, rank, procSize);
    } else {
        vec = getRandomVector(procSize, rank);
        toFile(initVectorFile, vec, n, rank, procSize);
    }

    if (createTestMode) {
        for (int q1 = 1; q1 <= n; q1++) {
            modelOneQubitEvolution(vec, H, n, q1);
            for (int q2 = q1 + 1; q2 <= n; q2++) {
                R[3][3] = exp(2 * M_PI * complexd(0, 1) / double(1 << (q2 - q1 + 1)));
                modelTwoQubitEvolution(vec, R, n, q1, q2);
            }
        }
    } else {
        for (int q1 = 1; q1 <= n; q1++) {
            oneQubitEvolution(vec, H, n, q1, rank, procSize);
            for (int q2 = q1 + 1; q2 <= n; q2++) {
                R[3][3] = exp(2 * M_PI * complexd(0, 1) / double(1 << (q2 - q1 + 1)));
                twoQubitEvolution(vec, R, n, q1, q2, rank, procSize);
            }
        }
    }

    if (testMode) {
        int procError = 0, error = 0;
        complexd* testVector = fromFile(resultFile, rank, procSize);

        for (int i = 0; i < procSize; i++) {
            if (abs(testVector[i].real() - vec[i].real()) > EPS || abs(testVector[i].imag() - vec[i].imag()) > EPS) {
                procError = 1;
                break;
            }
        }

        MPI_Reduce(&procError, &error, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            cout << (error ? "Error!" : "Correct!") << endl;
        }
    } else {
        toFile(resultFile, vec, n, rank, procSize);
    }

    MPI_Finalize();
    
    delete[] vec;
}