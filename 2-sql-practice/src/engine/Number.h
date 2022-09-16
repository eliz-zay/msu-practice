#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include <iostream>

#include "DataType.h"

using namespace std;

class Number: public DataType {
    private:
        long double value;

        Number(long double value);
    public:
        static Number* parse(string valueString);
        static Number* create(long double value);

        long double getValue();
        string toString();

        bool operator == (const Number &number) const;
        bool operator == (const long double numberValue) const;

        virtual bool operator > (const Number &number) const;
        virtual bool operator > (const long double numberValue) const;

        virtual bool operator < (const Number &dataType) const;
        virtual bool operator < (const long double numberValue) const;

        virtual bool operator >= (const Number &dataType) const;
        virtual bool operator >= (const long double numberValue) const;

        virtual bool operator <= (const Number &dataType) const;
        virtual bool operator <= (const long double numberValue) const;

        virtual bool operator != (const Number &dataType) const;
        virtual bool operator != (const long double numberValue) const;
};

#endif
