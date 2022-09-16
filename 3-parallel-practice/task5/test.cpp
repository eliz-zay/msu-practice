#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <math.h>

using namespace std;

double* fromBin(string fileName, int& n) {
    char type;
    int fd = open(fileName.c_str(), O_RDONLY, 0777);

    size_t tempN;

    read(fd, &type, sizeof(char));
    read(fd, &tempN, sizeof(size_t));
    read(fd, &tempN, sizeof(size_t));

    n = static_cast<int>(tempN);

    double* A = new double [n * n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            read(fd, &(A[i * n + j]), sizeof(double));
        }
    }

    close(fd);

    return A;
}

int main(int argc, char* argv[]) {
    string fileC;
    string fileExp;
    int n;

    if (argc != 3) {
        cout << "2 arguments required" << endl;
        return 0;
    }
    
    fileC = argv[1];
    fileExp = argv[2];

    double* C = fromBin(fileC, n);
    double* Exp = fromBin(fileExp, n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (fabs(C[i * n + j] - Exp[i * n + j]) > 0.0001) {
                cout << "wrong" << endl;
                return 0;
            }
        }
    }

    cout << "ok " << endl;
    return 0;
}