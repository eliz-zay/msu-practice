#include <mpi.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <string>

#include <complex>
#include <cmath>

#include "qubitEvolution.hpp"
#include "Gates.cpp"

#define EPS 1e-4

using namespace std;

typedef complex<double> complexd;

void print(complexd* v, unsigned long long procSize) {
    for (unsigned long long i = 0; i < procSize; i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;;
}

void parseArguments(int argc, char** argv, unsigned& n, bool& readMode, bool& testMode, bool& box, bool& canon, char*& readFile) {
    n = atoi(argv[1]);
    readMode = string(argv[2]).compare("read") == 0;
    testMode = string(argv[3]).compare("test") == 0;
    box = string(argv[4]).compare("box") == 0 || string(argv[4]).compare("both") == 0;
    canon = string(argv[4]).compare("canon") == 0 || string(argv[4]).compare("both") == 0;
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

bool areEqual(complexd* u, complexd* v, int rank, unsigned long long procSize) {
    int procError = 0, error = 0;

    for (int i = 0; i < procSize; i++) {
        if (abs(u[i].real() - v[i].real()) > EPS || abs(u[i].imag() - v[i].imag()) > EPS) {
            procError = 1;
            break;
        }
    }

    MPI_Reduce(&procError, &error, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return rank == 0 ? !error : false;
}

double getMeasure(complexd* u, unsigned long long procSize) {
    double sum = 0, localSum = 0;
    for (int i = 0; i < procSize; i++) {
        localSum += abs(u[i] * u[i]);
    }

    MPI_Reduce(&localSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return sum;
}

int main(int argc, char** argv) {
    bool readMode = false, testMode = false, box = false, canon = false;
    int rank, procNum;
    unsigned n;
    complexd* vec;

    char* initVectorFile = "init_vector.bin";
    char* resultFile = "result_vector.bin";
    char* readFile;

    parseArguments(argc, argv, n, readMode, testMode, box, canon, readFile);
    if (argc == 6) {
        initVectorFile = readFile;
    }

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);

    unsigned long long length = 1LLU << n;
    unsigned long long procSize = length / procNum;

    if ((box || canon) && procNum > 2) {
        if (rank == 0) {
            cout << "Black box works only on 1 or 2 processes." << endl;
        }
        exit(0);
    }

    if (box) {
        unsigned boxN = 2;
        unsigned long long length = 1LLU << boxN;
        unsigned long long procSize = length / procNum;

        length = 1LLU << n;
        procSize = length / procNum;

        bool check;
        complexd* in = new complexd [procSize];
        complexd* out = new complexd [procSize];
        complexd* result = new complexd [procSize];
        if (rank == 0) {
            in[0] = complexd(0, 1);
            in[1] = complexd(1, 0);
            if (procNum == 1) {
                in[2] = complexd(2, 0);
                in[3] = complexd(0, 1);
            }
        } else if (rank == 1) {
            in[0] = complexd(2, 0);
            in[1] = complexd(0, 1);
        }

        int q1 = 1;
        int q2 = 2;

        if (rank == 0) {
            cout << endl << "Black box" << endl << endl;
        }

        if (rank == 0) {
            result[0] = complexd(1.41421, 0.707107);
            result[1] = complexd(0.707107, 0.707107);
            if (procNum == 1) {
                result[2] = complexd(-1.41421, 0.707107);
                result[3] = complexd(0.707107, -0.707107);
            }
        } else if (rank == 1) {
            result[0] = complexd(-1.41421, 0.707107);
            result[1] = complexd(0.707107, -0.707107);
        }
        memcpy(out, in, sizeof(complexd) * procSize);
        Gates::H(out, n, q1, rank, procSize);
        check = areEqual(out, result, rank, procSize);
        if (rank == 0) {
            cout << (check ? "H:\tcorrect\n" : "H:\terror\n");
        }


        if (rank == 0) {
            result[0] = complexd(0.707107,0.707107);
            result[1] = complexd(-0.707107, 0.707107);
            if (procNum == 1) {
                result[2] = complexd(1.41421, 0.707107);
                result[3] = complexd(1.41421, -0.707107);
            }
        } else if (rank == 1) {
            result[0] = complexd(1.41421, 0.707107);
            result[1] = complexd(1.41421, -0.707107);
        }
        memcpy(in, out, procSize);
        Gates::Hn(out, n, rank, procSize);
        check = areEqual(out, result, rank, procSize);
        if (rank == 0) {
            cout << (check ? "Hn:\tcorrect\n" : "Hn:\terror\n");
        }


        if (rank == 0) {
            result[0] = complexd(1.41421, 0.707107);
            result[1] = complexd(1.41421, -0.707107);
            if (procNum == 1) {
                result[2] = complexd(0.707107,0.707107);
                result[3] = complexd(-0.707107,0.707107);
            }
        } else if (rank == 1) {
            result[0] = complexd(0.707107,0.707107);
            result[1] = complexd(-0.707107,0.707107);
        }
        memcpy(in, out, procSize);
        Gates::NOT(out, n, q1, rank, procSize);
        check = areEqual(out, result, rank, procSize);
        if (rank == 0) {
            cout << (check ? "NOT:\tcorrect\n" : "NOT:\terror\n");
        }


        if (rank == 0) {
            result[0] = complexd(1.41421, 0.707107);
            result[1] = complexd(1.41421, -0.707107);
            if (procNum == 1) {
                result[2] = complexd(-0.707107,-0.707107);
                result[3] = complexd(0.707107,-0.707107);
            }
        } else if (rank == 1) {
            result[0] = complexd(-0.707107,-0.707107);
            result[1] = complexd(0.707107,-0.707107);
        }
        memcpy(in, out, procSize);
        Gates::ROT(out, n, q1, rank, procSize);
        check = areEqual(out, result, rank, procSize);
        if (rank == 0) {
            cout << (check ? "ROT:\tcorrect\n" : "ROT:\terror\n");
        }


        if (rank == 0) {
            result[0] = complexd(1.41421, 0.707107);
            result[1] = complexd(1.41421, -0.707107);
            if (procNum == 1) {
                result[2] = complexd(0.707107,-0.707107);
                result[3] = complexd(-0.707107,-0.707107);
            }
        } else if (rank == 1) {
            result[0] = complexd(0.707107,-0.707107);
            result[1] = complexd(-0.707107,-0.707107);
        }
        memcpy(in, out, procSize);
        Gates::CNOT(out, n, q1, q2, rank, procSize);
        check = areEqual(out, result, rank, procSize);
        if (rank == 0) {
            cout << (check ? "CNOT:\tcorrect\n" : "CNOT:\terror\n");
        }


        if (rank == 0) {
            result[0] = complexd(1.41421, 0.707107);
            result[1] = complexd(1.41421, -0.707107);
            if (procNum == 1) {
                result[2] = complexd(0.707107,-0.707107);
                result[3] = complexd(0.707107,0.707107);
            }
        } else if (rank == 1) {
            result[0] = complexd(0.707107,-0.707107);
            result[1] = complexd(0.707107,0.707107);
        }
        memcpy(in, out, procSize);
        Gates::CROT(out, n, q1, q2, rank, procSize);
        check = areEqual(out, result, rank, procSize);
        if (rank == 0) {
            cout << (check ? "CROT:\tcorrect\n" : "CROT:\terror\n");
        }

        delete[] in;
        delete[] out;
        delete[] result;
    }

    if (canon) {
        double metrics = 0;

        vec = getRandomVector(procSize, rank);

        if (rank == 0) {
            cout << endl << "Canonization: vector norm" << endl << endl;
        }

        Gates::H(vec, n, 1, rank, procSize);
        metrics = getMeasure(vec, procSize);
        if (rank == 0) {
            cout << "H:\t" << metrics << endl;
        }

        Gates::Hn(vec, n, rank, procSize);
        metrics = getMeasure(vec, procSize);
        if (rank == 0) {
            cout << "Hn:\t" << metrics << endl;
        }

        Gates::NOT(vec, n, 1, rank, procSize);
        metrics = getMeasure(vec, procSize);
        if (rank == 0) {
            cout << "NOT:\t" << metrics << endl;
        }

        Gates::ROT(vec, n, 1, rank, procSize);
        metrics = getMeasure(vec, procSize);
        if (rank == 0) {
            cout << "ROT:\t" << metrics << endl;
        }

        Gates::CNOT(vec, n, 1, 2, rank, procSize);
        metrics = getMeasure(vec, procSize);
        if (rank == 0) {
            cout << "CNOT:\t" << metrics << endl;
        }

        Gates::CROT(vec, n, 1, 2, rank, procSize);
        metrics = getMeasure(vec, procSize);
        if (rank == 0) {
            cout << "CROT:\t" << metrics << endl;
        }
    }

    if (box || canon) {
        MPI_Finalize();
        exit(0);
    }

    if (readMode) {
        vec = fromFile(initVectorFile, rank, procSize);
    } else {
        vec = getRandomVector(procSize, rank);
        toFile(initVectorFile, vec, n, rank, procSize);
    }

    double start = MPI_Wtime();

    Gates::CROT(vec, n, 1, 2, rank, procSize);

    double end = MPI_Wtime();

    if (testMode) {
        complexd* testVector = fromFile(resultFile, rank, procSize);
        bool error = areEqual(testVector, vec, rank, procSize);

        if (rank == 0) {
            cout << (error ? "Error!" : "Correct!") << endl;
        }
    } else {
        toFile(resultFile, vec, n, rank, procSize);

        // if (rank == 0) {
        //     ofstream file("time.csv", ios::app);
        //     file << procNum << "\t" << end - start << endl;
        //     file.close();
        // }
    }

    MPI_Finalize();
    
    delete[] vec;
}