#ifndef VARCHAR_H
#define VARCHAR_H

#include <string>

#include "DataType.h"

using namespace std;

class Varchar: public DataType {
    private:
        string value;
        int length;

        Varchar(string value);
    public:
        static Varchar* parse(string valueString);
        static Varchar* create(string value);

        string getValue();
        string toString();

        bool operator == (const Varchar &varchar) const;
        bool operator == (const string stringValue) const;

        virtual bool operator > (const Varchar &varchar) const;
        virtual bool operator > (const string stringValue) const;

        virtual bool operator < (const Varchar &varchar) const;
        virtual bool operator < (const string stringValue) const;

        virtual bool operator >= (const Varchar &varchar) const;
        virtual bool operator >= (const string stringValue) const;

        virtual bool operator <= (const Varchar &varchar) const;
        virtual bool operator <= (const string stringValue) const;

        virtual bool operator != (const Varchar &varchar) const;
        virtual bool operator != (const string varchar) const;
};

#endif