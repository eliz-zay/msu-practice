#pragma once

#include "QueryObject.h"

QueryObject::QueryObject(QueryTypeEnum type, string table) {
    this->type = type;
    this->table = table;
}

QueryTypeEnum QueryObject::getType() {
    return this->type;
}

string QueryObject::getTable() {
    return this->table;
}
