#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string>

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

int main(int argc, char* argv[]) {
    int n;
    string fileA = "data/A.bin";
    string fileB = "data/B.bin";
    string fileC = "data/Exp.bin";

    if (argc == 2) {
        n = atoi(argv[1]);
    }

    double* A = new double [n * n];
    double* B = new double [n * n];
    double* C = new double [n * n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i * n + j] = rand() % 100;
            B[i * n + j] = rand() % 100;
            C[i * n + j] = 0;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }

    int fdA = open(fileA.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
    int fdB = open(fileB.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
    int fdC = open(fileC.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);


    write(fdA, "d", sizeof(char));
    write(fdB, "d", sizeof(char));
    write(fdC, "d", sizeof(char));
    write(fdA, &n, sizeof(size_t));
    write(fdB, &n, sizeof(size_t));
    write(fdC, &n, sizeof(size_t));
    write(fdA, &n, sizeof(size_t));
    write(fdB, &n, sizeof(size_t));
    write(fdC, &n, sizeof(size_t));
    for (int i = 0; i < n; ++i) {
        write(fdA, &A[i * n], sizeof(double) * n);
        write(fdB, &B[i * n], sizeof(double) * n);
        write(fdC, &C[i * n], sizeof(double) * n);
    }

    close(fdA);
    close(fdB);
    close(fdC);

    return 0;
}