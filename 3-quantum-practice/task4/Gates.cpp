#pragma once

#include "Gates.hpp"

#include "qubitEvolution.hpp"

void Gates::H(complexd* vec, unsigned n, unsigned q, int rank, unsigned long long procSize) {
    complexd H[2][2] = {{1 / sqrt(2), 1 / sqrt(2)}, {1 / sqrt(2), -1 / sqrt(2)}};
    Evolution::oneQubitEvolution(vec, H, n, q, rank, procSize);
}

void Gates::Hn(complexd* vec, unsigned n, int rank, unsigned long long procSize) {
    for (int q = 1; q <= n; q++) {
        Gates::H(vec, n, q, rank, procSize);
    }
}

void Gates::NOT(complexd* vec, unsigned n, unsigned q, int rank, unsigned long long procSize) {
    complexd NOT[2][2] = {{0, 1}, {1, 0}};
    Evolution::oneQubitEvolution(vec, NOT, n, q, rank, procSize);
}

void Gates::ROT(complexd* vec, unsigned n, unsigned q, int rank, unsigned long long procSize) {
    complexd ROT[2][2] = {{1, 0}, {0, -1}};
    Evolution::oneQubitEvolution(vec, ROT, n, q, rank, procSize);
}

void Gates::CNOT(complexd* vec, unsigned n, unsigned q1, unsigned q2, int rank, unsigned long long procSize) {
    complexd CNOT[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}};
    Evolution::twoQubitEvolution(vec, CNOT, n, q1, q2, rank, procSize);
}

void Gates::CROT(complexd* vec, unsigned n, unsigned q1, unsigned q2, int rank, unsigned long long procSize) {
    complexd CROT[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, -1}};
    Evolution::twoQubitEvolution(vec, CROT, n, q1, q2, rank, procSize);
}