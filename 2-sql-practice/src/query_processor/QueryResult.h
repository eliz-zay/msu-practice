#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include <string>
#include <vector>

using namespace std;

class QueryResult {
    protected:
        string table;
        int time;
    public:
        QueryResult(string table, int time);
        virtual string toString() = 0;
};

#endif