#include <omp.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <time.h>
#include <math.h>
#include <string>

double EPS = 0.001; // to check result matrix


using namespace std;

void printMatrix(double* matrix, int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
        printf("%7.4f ", matrix[i * size + j]);
    }
    printf("\n");
  }

  printf("\n");
}

void matrixInit(double* M, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            M[i * size + j] = rand() % 10;
        }
    }
}

void init(double* &A, double* &B, double* &C, int& size) {
    cout << "Size: ";
    cin >> size;

    A = new double [size * size];
    B = new double [size * size];
    C = new double [size * size];

    matrixInit(A, size);
    matrixInit(B, size);
}

void mult(double* A, double* B, double* C, int size) {
    double sum;
    int i, j, k;

    for (i = 0; i < size; ++i) {
        for (k = 0; k < size; ++k) {   
            sum = 0;
            for (j = 0; j < size; ++j) {
                sum += A[i * size + j] * B[j * size + k];
            }
            C[i * size + k] = sum;
        }
    }
}

void multOpenMP(double* A, double* B, double* C, int size, int threads) {
    double sum;
    int i, j, k;

    omp_set_num_threads(threads);

    #pragma omp parallel for private(j, k, sum)
    for (i = 0; i < size; ++i) {
        if (i == 0) {
            cout << endl << "Number of threads: " << omp_get_num_threads() << endl;
        }

        for (k = 0; k < size; ++k) {   
            sum = 0;
            for (j = 0; j < size; ++j) {
                sum += A[i * size + j] * B[j * size + k];
            }
            C[i * size + k] = sum;
        }

    }
}

void testAndPlot(string fileName, int maxThreads) {
    ofstream file(fileName.c_str(), ios::trunc);
    double begin, time;
    int maxSize;
    double *A, *B, *C;

    init(A, B, C, maxSize);

    for (int size = 200; size <= maxSize; size += 200) {
        for (int threads = 2; threads <= maxThreads; threads *= 2) {
            begin = omp_get_wtime();
            multOpenMP(A, B, C, size, threads);
            time = omp_get_wtime() - begin;

            file << threads << "\t" << size << "\t" << time << endl;
        }
        file << endl << endl;
    }

    delete[] A;
    delete[] B;
    delete[] C;

    file.close();
}

void printInfo(double* A, double* B, double* C, int size, double timeMP, double time) {
    cout << endl << "Time: " << timeMP << endl;
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
        int maxThreads = 160;
        string fileName = "data.csv";

        testAndPlot(fileName, maxThreads);
    } catch (exception& exc) {
        cout << exc.what() << endl;
    }
}