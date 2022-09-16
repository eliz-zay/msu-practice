#ifndef DROP_OBJECT_H
#define DROP_OBJECT_H

#include "QueryObject.h"

class DropObject: public QueryObject {
    public:
        DropObject(string table);

        string toString();
};

#endif
