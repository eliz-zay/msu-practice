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

void print(complexd* v, unsigned long long procSize) {
    for (unsigned long long i = 0; i < procSize; i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;;
}

void parseArguments(int argc, char** argv, double& noise, unsigned& n, bool& readMode, bool& testMode, char*& readFile) {
    noise = atof(argv[1]);
    n = atoi(argv[2]);
    readMode = string(argv[3]).compare("read") == 0;
    testMode = string(argv[4]).compare("test") == 0;
    if (argc == 6) {
        readFile = argv[5];
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

void toFile(char* outFile, complexd* B, unsigned long long n, int rank, unsigned long long procSize) {
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

void oneQubitEvolution(complexd* in, complexd* out, complexd U[2][2], unsigned n, unsigned q, int rank, unsigned long long procSize) {
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

double normalDisGenerate() {
    double s = 0.;
    for (int i = 0; i < 12; i++) {
        s += (double)rand() / RAND_MAX;
    }
    return s - 6.;
}

double getFidelity(complexd* idealVec, complexd* noisedVec, unsigned long long procSize, int rank) {
    MPI_Status status;
    double localDot = 0, globalDot = 0;
    for (int i = 0; i < procSize; i++) {
        localDot += abs(idealVec[i] * conj(noisedVec[i]));
    }
    MPI_Reduce(&localDot, &globalDot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalDot;
}

int main(int argc, char** argv) {
    bool readMode = false, testMode = false;
    int rank, procNum;
    unsigned n;
    complexd *procIn, *procOut, *procOutNoised;
    struct timeval start, stop;

    char* initVectorFile = "init_vector.bin";
    char* resultFile = "result_vector.bin";
    char* readFile;

    double noise, ksi, teta;
    complexd Hnoised[2][2];

    parseArguments(argc, argv, noise, n, readMode, testMode, readFile);
    if (argc == 6) {
        initVectorFile = readFile;
    }

    complexd H[2][2];
    complexd Hcoeff(1 / sqrt(2));
    H[0][0] = Hcoeff;
    H[0][1] = Hcoeff;
    H[1][0] = Hcoeff;
    H[1][1] = -Hcoeff;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);

    unsigned long long length = 1LLU << n;
    unsigned long long procSize = length / procNum;

    if (readMode) {
        procIn = fromFile(initVectorFile, rank, procSize);
    } else {
        procIn = getRandomVector(procSize, rank);
        // toFile(initVectorFile, procIn, n, rank, procSize);
    }

    procOut = new complexd[procSize];
    procOutNoised = new complexd[procSize];

    double begin = MPI_Wtime();

    for (int q = 1; q <= n; q++) { // n-Hadamard
        oneQubitEvolution(procIn, procOut, H, n, q, rank, procSize);
    }

    for (int q = 1; q <= n; q++) {
        if (rank == 0) {
            ksi = normalDisGenerate();
            teta = noise * ksi;
        }
        MPI_Bcast(&teta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        double cosTeta = cos(teta), sinTeta = sin(teta);
        Hnoised[0][0] = H[0][0] * cosTeta - H[0][1] * sinTeta;
        Hnoised[0][1] = H[0][0] * sinTeta + H[0][1] * cosTeta;
        Hnoised[1][0] = H[1][0] * cosTeta - H[1][1] * sinTeta;
        Hnoised[1][1] = H[1][0] * sinTeta + H[1][1] * cosTeta;
        oneQubitEvolution(procIn, procOutNoised, Hnoised, n, q, rank, procSize);
    }

    double time = MPI_Wtime() - begin;

    if (testMode) {
        /*
        bool* allErrors = new bool[procNum];
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
            for (int i = 0; i < procNum; i++) {
                if (allErrors[i]) {
                    error = true;
                    break;
                }
            }

            cout << (error ? "Error!" : "Correct") << endl;
        }

        delete[] allErrors;
        */
    } else {
        double fidelity = getFidelity(procOut, procOutNoised, procSize, rank);
        if (rank == 0) {
            cout << "Time: " << time << endl;
            cout << "Fidelity: " << fidelity << endl;
        }
    }

    MPI_Finalize();
    
    delete[] procOut;
    delete[] procOutNoised;
    delete[] procIn;
}