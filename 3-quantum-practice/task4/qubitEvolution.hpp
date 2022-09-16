#ifndef QUBIT_EVOL
#define QUBIT_EVOL

#include <mpi.h>

#include <iostream>

using namespace std;

typedef complex<double> complexd;

namespace Evolution {
    void oneQubitEvolution(complexd* vec, complexd U[2][2], unsigned n, unsigned q, int rank, unsigned long long procSize) {
        int firstIdx = rank * procSize;
        int shift = n - q;
        int pow = 1 << shift;
        int rankChange = firstIdx ^ pow;
        rankChange /= procSize;

        complexd* in = new complexd [procSize];
        memcpy(in, vec, sizeof(complexd) * procSize);

        if (rank != rankChange) {
            complexd* inNew = new complexd [procSize];
            MPI_Sendrecv(in, procSize, MPI_DOUBLE_COMPLEX, rankChange, 0, inNew, procSize, MPI_DOUBLE_COMPLEX, rankChange, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (rank > rankChange) { 
                for (int i = 0; i < procSize; i++) {
                    vec[i] = U[1][0] * inNew[i] + U[1][1] * in[i];
                }
            } else {
                for (int i = 0; i < procSize; i++) {
                    vec[i] = U[0][0] * in[i] + U[0][1] * inNew[i];
                }
            }

            delete[] inNew;
        } else {
            for (int i = 0; i < procSize; i++) {
                int i0 = i & ~pow;
                int i1 = i | pow;
                int iq = (i & pow) >> shift;
                vec[i] = U[iq][0] * in[i0] + U[iq][1] * in[i1];
            }
        }

        delete[] in;
    }

    void twoQubitEvolution(complexd* vec, complexd U[4][4], unsigned n, unsigned q1, unsigned q2, int rank, unsigned long long procSize) {
        complexd* in = new complexd [procSize];
        memcpy(in, vec, sizeof(complexd) * procSize);

        int firstIdx = rank * procSize;
        int shift1 = n - q1;
        int shift2 = n - q2;
        int pow1 = 1 << shift1;
        int pow2 = 1 << shift2;

        complexd* inVec00 = in;
        complexd* inVec01 = in;
        complexd* inVec10 = in;
        complexd* inVec11 = in;

        int rank00 = (firstIdx & ~pow1 & ~pow2) / procSize;
        int rank01 = (firstIdx & ~pow1 | pow2) / procSize;
        int rank10 = ((firstIdx | pow1) & ~pow2) / procSize;
        int rank11 = (firstIdx | pow1 | pow2) / procSize;

        if (rank != rank00) {
            inVec00 = new complexd [procSize];
            MPI_Sendrecv(in, procSize, MPI_DOUBLE_COMPLEX, rank00, 0, inVec00, procSize, MPI_DOUBLE_COMPLEX, rank00, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank != rank01) {
            inVec01 = new complexd [procSize];
            MPI_Sendrecv(in, procSize, MPI_DOUBLE_COMPLEX, rank01, 0, inVec01, procSize, MPI_DOUBLE_COMPLEX, rank01, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank != rank10) {
            inVec10 = new complexd [procSize];
            MPI_Sendrecv(in, procSize, MPI_DOUBLE_COMPLEX, rank10, 0, inVec10, procSize, MPI_DOUBLE_COMPLEX, rank10, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank != rank11) {
            inVec11 = new complexd [procSize];
            MPI_Sendrecv(in, procSize, MPI_DOUBLE_COMPLEX, rank11, 0, inVec11, procSize, MPI_DOUBLE_COMPLEX, rank11, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for	(int i = 0; i < procSize; i++) {
            int globali = firstIdx + i;
            int globali00 = globali & ~pow1 & ~pow2;
            int globali01 = globali & ~pow1 | pow2;
            int globali10 = (globali | pow1) & ~pow2;
            int globali11 = globali | pow1 | pow2;

            int i00 = globali00 - rank00 * procSize;
            int i01 = globali01 - rank01 * procSize;
            int i10 = globali10 - rank10 * procSize;
            int i11 = globali11 - rank11 * procSize;

            int iq1 = (globali & pow1) >> shift1;
            int iq2 = (globali & pow2) >> shift2;
            int iq = (iq1 << 1) + iq2;

            vec[i] = 
                U[iq][2 * 0 + 0] * inVec00[i00] + 
                U[iq][2 * 0 + 1] * inVec01[i01] + 
                U[iq][2 * 1 + 0] * inVec10[i10] + 
                U[iq][2 * 1 + 1] * inVec11[i11];
        }

        delete[] in;
        if (rank != rank00) {
            delete[] inVec00;
        }
        if (rank != rank01) {
            delete[] inVec01;
        }
        if (rank != rank10) {
            delete[] inVec10;
        }
        if (rank != rank11) {
            delete[] inVec11;
        }
    }

    void modelOneQubitEvolution(complexd* vec, complexd U[2][2], unsigned n, unsigned q) {
        unsigned long long length = 1LLU << n;

        complexd* in = new complexd [length];
        memcpy(in, vec, sizeof(complexd) * length);

        int shift = n - q;
        int pow = 1 << shift;
        
        for (int i = 0; i < length; i++) {
            int i0 = i & ~pow;
            int i1 = i | pow;
            int iq = (i & pow) >> shift;
            vec[i] = U[iq][0] * in[i0] + U[iq][1] * in[i1];
        }

        delete[] in;
    }

    void modelTwoQubitEvolution(complexd* vec, complexd U[4][4], unsigned n, unsigned q1, unsigned q2) {
        unsigned long long length = 1LLU << n;

        complexd* in = new complexd [length];
        memcpy(in, vec, sizeof(complexd) * length);

        int shift1 = n - q1;
        int shift2 = n - q2;
        int pow1 = 1 << shift1;
        int pow2 = 1 << shift2;

        for	(int i = 0; i < length; i++) {
            int i00 = i & ~pow1 & ~pow2;
            int i01 = i & ~pow1 | pow2;
            int i10 = (i | pow1) & ~pow2;
            int i11 = i | pow1 | pow2;

            int iq1 = (i & pow1) >> shift1;
            int iq2 = (i & pow2) >> shift2;
            int iq = (iq1 << 1) + iq2;

            vec[i] = 
                U[iq][2 * 0 + 0] * in[i00] + 
                U[iq][2 * 0 + 1] * in[i01] + 
                U[iq][2 * 1 + 0] * in[i10] + 
                U[iq][2 * 1 + 1] * in[i11];
        }

        delete[] in;
    }
};

#endif