#include <pthread.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>

using namespace std;

char* helpPrimes;
char* primes;
int sqrtLast, first, last;

void* threadFunc(void* params) {
    int* bounds = ((int *)params);
    int firstElement = bounds[0];
    int lastElement = bounds[1];

     for (int i = firstElement; i <= lastElement; i++) {
        primes[i - first] = '1';
    }

    for (int i = 2; i <= sqrtLast; i++) {
        if (helpPrimes[i] == '1') {
            int a = ceil((firstElement - i*i) * 1.0 / i);
            a = a < 0 ? 0 : a;
            for (int j = i*i + a * i; j <= lastElement; j += i) {
                primes[j - first] = '0';
            }
        }
    }

    delete[] bounds;

    pthread_exit(0);
}

void printInfo(float time, int res, int first, int last) {
    cout << "Time: " << time << endl;
    cout << res << " prime numbers between " << first << " and " << last << endl;
}

void toOutFile(string fileName, int first, int last) {
    if (fileName.length() == 0) return;

    ofstream file;
    file.open(fileName.c_str(), ios::out | ios::trunc);

    for (int i = 0; i < last - first; i++) {
        if (primes[i]) {
            file << i << " ";
        }
    }

    file.close();
}

void toTimeFile(string fileName, int threadNum, int time) {
    ofstream totalFile;
    totalFile.open(fileName.c_str(), ios::out | ios::app);
    totalFile << threadNum << "\t" << time << endl;
    totalFile.close();
}

void parseCommand(int argc, char* argv[], int& threadNum, int& first, int& last, string& fileName) {
    if (argc != 5) {
        cout << "4 parameters required" << endl;
        exit(0);
    }

    threadNum = atoi(argv[1]);
    first = atoi(argv[2]);
    last = atoi(argv[3]);
    fileName = argv[4];
}

int main(int argc, char *argv[]) {
    int threadNum, res;
    string outFileName;
    string timeCsvName = "time.csv";

    parseCommand(argc, argv, threadNum, first, last, outFileName);

    cout << "Number of threads: " << threadNum << endl << endl;

	struct timeval start, end;
	gettimeofday(&start, NULL);

    int len = last - first;
    sqrtLast = int(sqrt(last));

    helpPrimes = new char [sqrtLast + 1];

    helpPrimes[0] = '0';
    helpPrimes[1] = '0';
    for (int i = 2; i <= sqrtLast; ++i) {
        helpPrimes[i] = '1';
    }

    for (int i = 2; i <= sqrtLast; ++i) {
        if (helpPrimes[i] == '1') {
            for (int j = i*i; j <= sqrtLast; j += i) {
                helpPrimes[j] = '0';
            }
        }
    }

    int partSize = ceil(len * 1.0 / threadNum);

    pthread_t threadIds[threadNum];
    pthread_attr_t threadAttributes;

    pthread_attr_init(&threadAttributes);

    primes = new char [(int)(ceil((last - first) * 1.0 / threadNum) * threadNum)];

    for (int i = 0; i < threadNum; i++) {
        int* bounds = new int [2];
        bounds[0] = i * partSize + first;
        bounds[1] = i == threadNum - 1 ? len - 1 + first : bounds[0] + partSize - 1;
        pthread_create(&threadIds[i], &threadAttributes, threadFunc, (void*)bounds);
    }

    for (int i = 0; i < threadNum; i++) {
        pthread_join(threadIds[i], NULL);
    }

    if (first == 1) {
        primes[0] = '0';
    }

	gettimeofday(&end, NULL);
    float time = ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000) * 1.0 / 1000;

    res = count(primes, &primes[last - first], '1');

    printInfo(time, res, first, last);
    toOutFile(outFileName, first, last);
    toTimeFile(timeCsvName, threadNum, time);

    delete[] primes;
    delete[] helpPrimes;
}
