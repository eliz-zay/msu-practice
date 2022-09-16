#include <mpi.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <vector>
#include <math.h>
#include <time.h>
#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

void printVec(int* vec, int len, int procRank = -1) {
    if (procRank >= 0) {
        cout << procRank << ": ";
    }
    for (int i = 0; i < len; ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

void initProcess(int &procNum, int &procRank) {
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
}

pair<int, int> getCoord(int procRank, int size) {
    int x = (int)(procRank / size);
    int y = (int)(procRank % size);
    return make_pair(x, y);
}

int getRank(pair<int, int> coord, int size) {
    if (coord.first < 0 || coord.first >= size || coord.second < 0 || coord.second >= size) {
        return -1;
    }
    return coord.first * size + coord.second;
}

int* genArray(int procRank) {
    int* arr = new int[procRank];
    for (int i = 0; i < procRank; ++i) {
        arr[i] = rand() % 100;
    }

    return arr;
}

pair<int, int> moveLeft(pair<int, int> coord) {
    int x = coord.first;
    int y = coord.second - 1;
    return make_pair(x, y);
}

pair<int, int> moveUp(pair<int, int> coord) {
    int x = coord.first - 1;
    int y = coord.second;
    return make_pair(x, y);
}

pair<int, int> moveRight(pair<int, int> coord) {
    int x = coord.first;
    int y = coord.second + 1;
    return make_pair(x, y);
}

pair<int, int> moveDown(pair<int, int> coord) {
    int x = coord.first + 1;
    int y = coord.second;
    return make_pair(x, y);
}

int getTargetRank(pair<int, int> coord, int size) {
    bool isDiagonal = coord.first == coord.second;
    bool isAboveDiagonal = coord.first < coord.second;

    pair<int, int> targetCoord;

    if (isDiagonal) {
        if (coord.first || coord.second) {
            if (coord.second < size / 2) { // если выше побочной диагонали - налево
                targetCoord = moveLeft(coord);
            } else {
                targetCoord = moveUp(coord);
            }  
        }
    } else if (isAboveDiagonal) {
        if (coord.first != 0) {
            targetCoord = moveUp(coord);
        } else {
            targetCoord = moveLeft(coord);
        }
    } else {
        if (coord.second != 0) {
            targetCoord = moveLeft(coord);
        } else {
            targetCoord = moveUp(coord);
        }
    }

    int targetRank = getRank(targetCoord, size);

    return targetRank;
}

pair<int, int> getSourceRanks(pair<int, int> coord, int size) {
    pair<int, int> sourceRanks;
    pair<int, int> coordUnder = moveDown(coord);
    pair<int, int> coordOnRight = moveRight(coord);

    int procRank = getRank(coord, size);
    int rankUnder = getRank(coordUnder, size);
    int rankOnRight = getRank(coordOnRight, size);

    int targetRankUnder = getTargetRank(coordUnder, size);
    int targetRankOnRight = getTargetRank(coordOnRight, size);

    if (targetRankUnder == procRank && targetRankOnRight == procRank) {
        sourceRanks = make_pair(rankUnder, rankOnRight);
    } else {
        int validRank = targetRankUnder == procRank ? rankUnder : (targetRankOnRight == procRank ? rankOnRight : -1);
        sourceRanks = make_pair(validRank, -1);
    }

    return sourceRanks;
}

/*
* Возвращает число Recv
* first: суммарное кол-во recv
* second: кол-во recv от каждого из 2 источников
*/
pair<int, pair<int, int> > getRecvNum(pair<int, int> coord, int size) {
    pair<int, int> sourceRanks = getSourceRanks(coord, size);

    if (sourceRanks.first == -1 && sourceRanks.second == -1) {
        return make_pair(0, make_pair(0, 0));
    }

    if (sourceRanks.first != -1 && sourceRanks.second != -1) {
        pair<int, int> sourceCoord1 = getCoord(sourceRanks.first, size);
        pair<int, int> sourceCoord2 = getCoord(sourceRanks.second, size);

        int recvNum1 = getRecvNum(sourceCoord1, size).first;
        int recvNum2 = getRecvNum(sourceCoord2, size).first;
        int recvNum = max(recvNum1, recvNum2) + 1;

        return make_pair(recvNum, make_pair(recvNum1 + 1, recvNum2 + 1));
    }

    int source = sourceRanks.first != -1 ? sourceRanks.first : sourceRanks.second;
    pair<int, int> sourceCoord = getCoord(source, size);

    int recvNum = getRecvNum(sourceCoord, size).first + 1;

    return make_pair(recvNum, make_pair(0, 0));
}

void initMsg(int* localArray, int length, int offset, int*& msg, int& msgLength) {
    msgLength = length + 3;
    msg = new int[msgLength];

    msg[0] = length + 3;
    msg[1] = length;
    msg[2] = offset;
    
    for (int i = 0; i < length; ++i) {
        msg[i + 3] = localArray[i];
    }
}

void genMsg(int* buffer1, int* buffer2, int maxLength, int*& msg, int& msgLength) {
    if (buffer1 == NULL) {
        throw invalid_argument("genMsg: buffer is NULL\n");
    }

    if (buffer2 == NULL) {
        msg = buffer1;
        msgLength = buffer1[0];
        return;
    }

    int length1 = buffer1[0] - 1;
    int length2 = buffer2[0] - 1;

    msgLength = buffer1[0] + buffer2[0] - 1;
    msg = new int[msgLength];

    msg[0] = msgLength;
    
    for (int i = 0; i < length1; ++i) {
        msg[i + 1] = buffer1[i + 1];
    }
    for (int i = 0; i < length2; ++i) {
        msg[i + 1 + length1] = buffer2[i + 1];
    }
}

void parseMsgs(vector<int*> msgArray, int*& result, int& totalLength) {
    vector<pair<pair<int, int>, int*> > procArrays; // массивы процессов с длинами и смещениями
    totalLength = 0; // макс смещение + соотв длина
    int maxOffset = 0;

    for (int* msg: msgArray) {
        const int msgLength = msg[0];

        int blockStart = 1; // блок: длина R + смещение + массив длины R
        int offset;
        int length;

        while (blockStart < msgLength) {
            length = msg[blockStart];
            offset = msg[blockStart + 1];
            procArrays.push_back(make_pair(
                make_pair(length, offset),
                msg + blockStart + 2
            ));

            if (offset > maxOffset) {
                maxOffset = offset;
                totalLength = offset + length;
            }
            blockStart += length + 2;
        }
    }

    result = new int[totalLength];
    for (int i = 0; i < totalLength; ++i) {
        result[i] = 0;
    }

    for (pair<pair<int, int>, int*> item: procArrays) {
        int length = item.first.first;
        int offset = item.first.second;
        int* array = item.second;

        for (int i = 0; i < length; ++i) {
            result[offset + i] = array[i];
        }
    }
}

void gather(int procRank, int size, pair<int, int> coord, int* localArray, int offset, int*& result, int& resLength) {
    const int targetRank = getTargetRank(coord, size);
    pair<int, int> sourceRanks = getSourceRanks(coord, size);

    pair<int, pair<int, int> > recvNumTotal = getRecvNum(coord, size);
    const int recvNum = recvNumTotal.first;
    int recvNum1 = recvNumTotal.second.first;
    int recvNum2 = recvNumTotal.second.second;

    const bool recvFromOne = sourceRanks.first == -1 && sourceRanks.second != -1 || sourceRanks.first != -1 && sourceRanks.second == -1;

    const int procNum = size * size;
    const int maxLength = (1 + procNum) * procNum / 4 + procNum;

    int* buffer1 = new int[maxLength];
    int* buffer2 = new int[maxLength];
    int* msg;
    int msgLength;

    vector<int*> msgArray; // массив с сообщениями на 0 процессе

    if (procRank > 0) {
        initMsg(localArray, procRank, offset, msg, msgLength);
    }
    
    for (int i = 0; i <= recvNum; ++i, --recvNum1, --recvNum2) {
        bool recv1 = false;
        bool recv2 = false;

        if (procRank > 0) {
            MPI_Send(msg, msgLength, MPI_INT, targetRank, 0, MPI_COMM_WORLD);
        }

        if (sourceRanks.first != -1 && ((recvFromOne && i < recvNum) || (recvNum1 > 0))) {
            MPI_Recv(buffer1, maxLength, MPI_INT, sourceRanks.first, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            recv1 = true;
        }

        if (sourceRanks.second != -1 && ((recvFromOne && i < recvNum)|| (recvNum2 > 0))) {
            MPI_Recv(buffer2, maxLength, MPI_INT, sourceRanks.second, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            recv2 = true;
        }
        
        genMsg(
            recv1 ? buffer1 : buffer2,
            recv1 && recv2 ? buffer2 : NULL,
            maxLength,
            msg,
            msgLength
        );
        
        if (procRank == 0 && i != recvNum) {
            if (recv1) {
                cout << buffer1[0] << ": ";
                printVec(buffer1, buffer1[0]);
            }
            if (recv2) {
                cout << buffer2[0] << ": ";
                printVec(buffer2, buffer2[0]);
            }
            msgArray.push_back(msg);
        }
    }

    if (procRank == 0) {
        parseMsgs(msgArray, result, resLength);
    }

    delete[] buffer1;
    delete[] buffer2;
}

int main(int argc, char* argv[]) {
    try {
        int* result;
        int resLength;

        int procNum;
        int procRank;

        MPI_Init(&argc, &argv);
        initProcess(procNum, procRank);

        const int size = sqrt(procNum);

        pair<int, int> coord = getCoord(procRank, size);
        int* localArray = genArray(procRank);
        int offset = procRank * (procNum - 1); // mock integer

        // printVec(localArray, procRank, procRank);

        gather(procRank, size, coord, localArray, offset, result, resLength);

        if (procRank == 0) {
            for (int i = 0; i < 16; ++i) {
                // printVec(result + 15 * i, 15);
            }
        }

        MPI_Finalize();

    } catch (exception& exc) {
        MPI_Finalize();
        cout << exc.what() << endl;
    }
}