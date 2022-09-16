#ifndef TABLE_IO_H
#define TABLE_IO_H

#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "TableField.h"
#include "DataType.h"

#include "EngineException.h"

using namespace std;

namespace TableIO {
    enum TableModeEnum {
        CREATE_WRITE_MODE = O_CREAT | O_EXCL | O_WRONLY,
        READ_MODE = O_RDONLY,
        WRITE_MODE = O_WRONLY | O_APPEND,
        READ_WRITE_MODE = O_RDWR
    };

    enum SeekWhence {
        START_WHENCE = SEEK_SET,
        CURRENT_WHENCE = SEEK_CUR,
        END_WHENCE = SEEK_END
    };

    int lastReadBytes = 0;

    void seek(int tableFD, SeekWhence whence, int offset) throw (EngineException);
    int getFD(string tableName, TableModeEnum mode) throw (EngineException);
    void closeFD(int tableFD) throw (EngineException);

    int readFieldsNumber(int tableFD) throw (EngineException);
    TableField* readTableField(int tableFD, int& bytesRead = lastReadBytes) throw (EngineException);
    DataType* readTableValue(int tableFD, TableField* field, int& bytesRead = lastReadBytes) throw (EngineException);
    bool readRowDeletedFlag(int tableFD, int& bytesRead = lastReadBytes) throw (EngineException);

    int writeFieldsNumber(int tableFD, int fieldsNumber) throw (EngineException);
    void writeTableField(int tableFD, TableField* field) throw (EngineException);
    void writeTableValue(int tableFD, TableField* field, DataType* value) throw (EngineException);
    void writeRowDeletedFlag(int tableFD, bool deleted) throw (EngineException);
    void deleteTable(string tableName) throw (EngineException);
};

#endif