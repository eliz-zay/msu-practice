#include <omp.h>

#include <iostream>
#include <fstream>
#include <complex>

typedef std::complex<double> complexd;

using namespace std;

void print(complexd* v, int len) {
    for (int i = 0; i < len; i++) {
        cout << v[i] << endl;
    }
}

complexd* getAdamarMatrix() {
    complexd* U = new complexd [4];
    complexd k(1/sqrt(2));

    for (int i = 0; i < 3; i++) {
        U[i] = k;
    }
    U[3] = -k;

    return U;
}

complexd* getRandomVector(int length) {
    complexd* vector = new complexd [length];
    double norm = 0.;

    for (int i = 0; i < length; i++) {
        vector[i] = complexd((double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
        norm += abs(vector[i]) * abs(vector[i]);
    }

    norm = sqrt(norm);

    for (int i = 0; i < length; i++) {
        vector[i] /= norm;
    }

    return vector;
}

complexd* oneQubitConversion(complexd* in, complexd* U, int q, int nqubits, int threads, double* time) {
    const int matrixDim = 2;
    int length = 1 << nqubits;
    complexd* out = new complexd [length];

	int shift = nqubits - q;
	int pow2q = 1 << shift;

    double time0 = omp_get_wtime();
    int i, i0, i1, iq;
    #pragma omp parallel for private(i, i0, i1, iq) shared(length,pow2q, shift, in, out, U) num_threads(threads)
	for	(i = 0; i < length; i++) {
		i0 = i & ~pow2q;
		i1 = i | pow2q;
		iq = (i & pow2q) >> shift;
		out[i] = U[iq * matrixDim] * in[i0] + U[iq * matrixDim + 1] * in[i1];
	}

    *time = omp_get_wtime() - time0;

    return out;
}

void parseArguments(int argc, char* argv[], int* qNum, int* nqubits, int* threads) {
    if (argc != 4) {
        cerr << "3 arguments required!" << endl;
        exit(0);
    }

    *qNum = atoi(argv[1]);
    *nqubits = atoi(argv[2]);
    *threads = atoi(argv[3]);
}

int main(int argc, char* argv[]) {
    int qNum, threads, nqubits;
    double time;

    parseArguments(argc, argv, &qNum, &nqubits, &threads);
    int len = 1 << nqubits;

    complexd* U = getAdamarMatrix();
    complexd* a = getRandomVector(len);
    complexd* b = oneQubitConversion(a, U, qNum, nqubits, threads, &time);

    cout << "Qubit to convert: " << qNum << endl;
    cout << "Qubits: " << nqubits << endl;
    cout << "Threads: " << threads << " -> " << time << " sec" << endl << endl;

    ofstream file("time.csv", ios::app);
    file << time << endl;
    file.close();

    delete[] U;
    delete[] a;
    delete[] b;
}