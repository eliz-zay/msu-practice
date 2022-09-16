#ifndef GATES
#define GATES

using namespace std;

typedef complex<double> complexd;

namespace Gates {
    void H(complexd* vec, unsigned n, unsigned q, int rank, unsigned long long procSize);
    void Hn(complexd* vec, unsigned n, int rank, unsigned long long procSize);
    void NOT(complexd* vec, unsigned n, unsigned q, int rank, unsigned long long procSize);
    void ROT(complexd* vec, unsigned n, unsigned q, int rank, unsigned long long procSize);
    void CNOT(complexd* vec, unsigned n, unsigned q1, unsigned q2, int rank, unsigned long long procSize);
    void CROT(complexd* vec, unsigned n, unsigned q1, unsigned q2, int rank, unsigned long long procSize);
};

#endif
