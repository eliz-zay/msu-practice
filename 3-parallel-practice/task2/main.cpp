#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <time.h>
#include <string>

#include <papi.h>

using namespace std;

float** mult (float** A, float** B, int rowsA, int colsA, int rowsB, int blockSize, int mode) {
    float** C = new float*[rowsA];
    for (int i = 0; i < rowsA; ++i) {
        C[i] = new float[rowsB];
        for (int j = 0; j < rowsB; ++j) {
            C[i][j] = 0;
        }
    }

    if (mode == 1) { // ijk
        for (int iBegin = 0; iBegin < rowsA; iBegin += blockSize) {
            int iEnd = (iBegin + blockSize) > rowsA ? rowsA : iBegin + blockSize;

            for (int jBegin = 0; jBegin < colsA; jBegin += blockSize) {
                int jEnd = (jBegin + blockSize) > colsA ? colsA : jBegin + blockSize;

                for (int kBegin = 0; kBegin < rowsB; kBegin += blockSize) {
                    int kEnd = (kBegin + blockSize) > rowsB ? rowsB : kBegin + blockSize;

                    for (int i = iBegin; i < iEnd; ++i) {
                        for (int j = jBegin; j < jEnd; ++j) {
                            for (int k = kBegin; k < kEnd; ++k) {
                                C[i][j] += A[i][k] * B[k][j];
                            }
                        }
                    }
                }
            }
        }
    } else { //ikj
        for (int iBegin = 0; iBegin < rowsA; iBegin += blockSize) {
            int iEnd = (iBegin + blockSize) > rowsA ? rowsA : iBegin + blockSize;

            for (int kBegin = 0; kBegin < rowsB; kBegin += blockSize) {
                int kEnd = (kBegin + blockSize) > rowsB ? rowsB : kBegin + blockSize;

                for (int jBegin = 0; jBegin < colsA; jBegin += blockSize) {
                    int jEnd = (jBegin + blockSize) > colsA ? colsA : jBegin + blockSize;

                    for (int i = iBegin; i < iEnd; ++i) {
                        for (int k = kBegin; k < kEnd; ++k) {
                            float elem = A[i][k];
                            for (int j = jBegin; j < jEnd; ++j) {
                                C[i][j] += elem * B[k][j];
                            }
                        }
                    }
                }
            }
        }
    }

    return C;
}

float** fromBin (const char* fileName, size_t& rows, size_t& cols) {
    int fd = open(fileName, O_RDONLY, 0777);

    read(fd, &rows, sizeof(size_t));
    read(fd, &cols, sizeof(size_t));

    float** matrix = new float*[rows];

    for (size_t i = 0; i < rows; ++i) {
        matrix[i] = new float[cols];
        read(fd, matrix[i], sizeof(float) * cols);
    }

    close(fd);

    return matrix;
}

void toBin (const char* fileName, float** matrix, size_t rows, size_t cols) {
    int fd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, 0777);

    write(fd, &rows, sizeof(size_t));
    write(fd, &cols, sizeof(size_t));

    for (size_t i = 0; i < rows; ++i) {
        write(fd, matrix[i], sizeof(float) * cols);
    }

    close(fd);
}

void print (float** matrix, size_t rows, size_t cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void parseCommand (int argc, char** argv, char** fileA, char** fileB, char** fileC, int* blockSize, int* mode) {
    if (argc != 6) {
        throw new invalid_argument("Wrong number of arguments");
    }

    *fileA = argv[1];
    *fileB = argv[2];
    *fileC = argv[3];
    *blockSize = atoi(argv[4]);
    *mode = atoi(argv[5]);

    if (*blockSize == 0) {
        throw new invalid_argument("4 argument must be unsigned integer");
    } else if (*mode != 1 && *mode != 2) {
        throw new invalid_argument("5 argument must be 1 or 2");
    }
}

void printInfo (size_t rowsA, size_t colsA, size_t rowsB, size_t colsB, int blockSize, int mode, long long papiValues[5], float time) {
    cout << "Block size: " << blockSize << endl;
    cout << "Multiplication mode: " << (mode == 1 ? "ijk" : "ikj") << endl;

    cout << "Sizes: " << rowsA << "x" << colsA;
    cout << " * " << rowsB << "x" << colsB;
    cout << " => " << rowsA << "x" << colsB << endl;
        
    cout << "Time: " << ((float)time)/CLOCKS_PER_SEC << " sec" << endl;

    cout << "L1 cache misses: " << papiValues[0] << endl;
    cout << "L2 cache misses: " << papiValues[1] << endl;
    cout << "Total cycles: " << papiValues[2] << endl;
    cout << "TLB: " << papiValues[3] << endl;
}

int main (int argc, char** argv) {
    try {
        char *fileA, *fileB, *fileC;
        int blockSize, mode;

        parseCommand(argc, argv, &fileA, &fileB, &fileC, &blockSize, &mode);

        float **A, **B;
        size_t rowsA, colsA;
        size_t rowsB, colsB;

        A = fromBin(fileA, rowsA, colsA);
        B = fromBin(fileB, rowsB, colsB);

        if (colsA != rowsB) {
            throw logic_error("Number of A columns are not equal number B rows");
        }

        clock_t time = 0;

        int papiEvents[4] = { PAPI_L1_DCM, PAPI_L2_DCM, PAPI_TOT_CYC, PAPI_TLB_TL };
        long long papiValues[4];
        int papiStatus;

        papiStatus = PAPI_start_counters(papiEvents, 4);
        if (papiStatus != PAPI_OK) {
            cerr << PAPI_strerror(papiStatus) << endl;
            return 1;
        }

        float** C = mult(A, B, rowsA, colsA, colsB, blockSize, mode);

        time = clock() - time;
        papiStatus = PAPI_read_counters(papiValues, 4);
        if (papiStatus != PAPI_OK) {
            cerr << PAPI_strerror(papiStatus) << endl;
            return 1;
        }

        printInfo(rowsA, colsA, rowsB, colsB, blockSize, mode, papiValues, time);

        toBin(fileC, C, rowsA, colsB);

        return 0;

    } catch (exception& exc) {

        cerr << exc.what() << endl;

    }
}