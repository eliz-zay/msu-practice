#pragma once

#include "TableIO.h"

#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "TableField.cpp"
#include "NumberField.cpp"
#include "VarcharField.cpp"

#include "DataType.cpp"
#include "Number.cpp"
#include "Varchar.cpp"

#include "DataTypeEnum.h"

#include "Config.h"

#include "EngineStatusEnum.h"
#include "EngineException.cpp"

using namespace std;

/*
 * Moves file pointer by given offset using given whence
 * @param tableFD - descriptor of given table
 * @param whence - whence mode
 * @param offset - offset the pointer will be moved
 */
void TableIO::seek(int tableFD, SeekWhence whence, int offset) throw (EngineException) {
    int result = lseek(tableFD, offset, SEEK_CUR);
    if (result == -1) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
}

/*
 * Opens table file with given given mode and returns its file decriptor
 * @param tableName - name of file
 * @param mode - file mode to be opened
 * @throws EngineException if file couldn't be created/opened
 * @returns file descriptor of new table
 */
int TableIO::getFD(string tableName, TableModeEnum mode) throw (EngineException) {
    int tableFd = open(
        (Config::STORAGE_LOCATION + tableName + Config::TABLE_FILE_EXTENSION).c_str(),
        mode,
        0777);

    if (tableFd == -1) {
        if (errno == EEXIST) {
            throw EngineException(EngineStatusEnum::TableAlreadyExists);
        } else if (errno == ENOENT) {
            throw EngineException(EngineStatusEnum::TableDoesNotExist);
        } else if (errno == EACCES) {
            throw EngineException(EngineStatusEnum::NoPermission);
        } else {
            throw EngineException(EngineStatusEnum::TableStructureCorrupted);
        }
    }

    return tableFd;
}

/*
 * Closes given file descriptor
 * @param tableFD - descriptor of given file
 * @throws EngineException if file couldn't be closed
 */
void TableIO::closeFD(int tableFD) throw (EngineException) {
    int result = close(tableFD);

    if (result == -1) {
        throw EngineException(EngineStatusEnum::BadTableDescriptor);
    }
}

/*
 * Reads fields number of table
 * @param tableFD - file descriptor of table
 * @throws EngineException if couldn't read from file
 * @returns fields number
 */
int TableIO::readFieldsNumber(int tableFD) throw (EngineException) {
    int fieldsNumber;

    int result = read(tableFD, &fieldsNumber, sizeof(int));
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }

    return fieldsNumber;
}

/*
 * Reads table field
 * @param tableFD - file descriptor of table
 * @param bytesRead - reference which will contain number of bytes written
 * @throws EngineException if couldn't read from file
 * @returns pointer to table field
 */
TableField *TableIO::readTableField(int tableFD, int &bytesRead) throw (EngineException) {
    bytesRead = 0;

    int nameLength;
    char name[TableField::MAX_FIELD_LENGTH];
    DataTypeEnum type;

    int result;

    result = read(tableFD, &nameLength, sizeof(int));
    if (result < 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
    bytesRead += result;

    result = read(tableFD, &name, nameLength * sizeof(char));
    if (result < 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
    bytesRead += result;
    name[nameLength] = '\0';

    result = read(tableFD, &type, sizeof(int));
    if (result < 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
    bytesRead += result;

    switch (type) {
        case DataTypeEnum::NUMBER: {
            return new NumberField(name);
        }
        case DataTypeEnum::VARCHAR: {
            int length;
            result = read(tableFD, &length, sizeof(int));
            if (result < 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }
            bytesRead += result;
            return new VarcharField(name, length);
        }
        default:
            throw EngineException(EngineStatusEnum::WrongFieldType);
    }
}

/*
 * Reads table value
 * @param tableFD - file descriptor of table
 * @param type - value type
 * @param bytesRead - reference which will contain number of bytes written
 * @throws EngineException if couldn't read from file
 * @returns pointer to value
 */
DataType *TableIO::readTableValue(int tableFD, TableField *field, int &bytesRead) throw (EngineException) {
    int result;
    bytesRead = 0;

    switch (field->getType()) {
        case DataTypeEnum::NUMBER: {
            long double numberValue;

            result = read(tableFD, &numberValue, sizeof(long double));
            if (result == 0) {
                return nullptr;
            } else if (result < 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }

            bytesRead += result;
            return Number::create(numberValue);
        }
        case DataTypeEnum::VARCHAR: {
            VarcharField *varcharField = dynamic_cast<VarcharField *>(field);

            char varcharValue[varcharField->getLength()];
            int length;

            result = read(tableFD, &length, sizeof(int));
            if (result == 0) {
                return nullptr;
            } else if (result < 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }
            bytesRead += result;

            result = read(tableFD, varcharValue, varcharField->getLength() * sizeof(char));
            if (result < 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }
            bytesRead += result;

            varcharValue[length] = '\0';
            return Varchar::create(varcharValue);
        }
        default:
            throw EngineException(EngineStatusEnum::WrongValueType);
    }
}

/*
 * Reads row "deleted" flag
 * @param tableFD - file descriptor containing table
 * @param bytesRead - reference which will contain number of bytes written
 * @throws EngineException
 */
bool TableIO::readRowDeletedFlag(int tableFD, int &bytesRead) throw (EngineException) {
    bool deleted;
    bytesRead = read(tableFD, &deleted, sizeof(bool));
    if (bytesRead <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
    return deleted;
}

/*
 * Writes table fields number
 * @param tableFD - file descriptor of table
 * @param fieldsNumber - fields number
 * @throws EngineException if couldn't write to file
 * @returns bytes written
 */
int TableIO::writeFieldsNumber(int tableFD, int fieldsNumber) throw (EngineException) {
    int result = write(tableFD, &fieldsNumber, sizeof(int));
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
    return result;
}

/*
 * Writes table field
 * @param tableFD - file descriptor of table
 * @param field
 * @throws EngineException if couldn't write to file
 */
void TableIO::writeTableField(int tableFD, TableField *field) throw (EngineException) {
    int nameLength = field->getName().length();
    const char *name = field->getName().c_str();
    int type = field->getType();

    int result;

    result = write(tableFD, &nameLength, sizeof(int));
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }

    result = write(tableFD, name, nameLength * sizeof(char));
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }

    result = write(tableFD, &type, sizeof(int));
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }

    switch (field->getType()) {
        case DataTypeEnum::NUMBER:
            break;
        case DataTypeEnum::VARCHAR: {
            VarcharField *varcharField = dynamic_cast<VarcharField *>(field);
            int length = varcharField->getLength();

            result = write(tableFD, &length, sizeof(int));
            if (result <= 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }

            break;
        }
        default:
            throw EngineException(EngineStatusEnum::WrongFieldType);
    }
}

/*
 * Writes table value
 * @param tableFD - file descriptor of table
 * @param value
 * @throws EngineException if couldn't write to file
 */
void TableIO::writeTableValue(int tableFD, TableField *field, DataType *value) throw (EngineException) {
    int result;

    switch (value->getType()) {
        case DataTypeEnum::NUMBER: {
            Number *number = dynamic_cast<Number *>(value);
            long double numberValue = number->getValue();

            result = write(tableFD, &numberValue, sizeof(long double));

            if (result <= 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }

            break;
        }
        case DataTypeEnum::VARCHAR: {
            Varchar *varchar = dynamic_cast<Varchar *>(value);
            VarcharField *varcharField = dynamic_cast<VarcharField *>(field);

            string varcharValue = varchar->getValue();
            int length = varcharValue.size();

            if (length > varcharField->getLength()) {
                throw EngineException(EngineStatusEnum::VarcharValueTooLong);
            }

            result = write(tableFD, &length, sizeof(int));
            if (result <= 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }

            varcharValue += string(varcharField->getLength() - length, ' ');
            result = write(tableFD, varcharValue.c_str(), varcharField->getLength() * sizeof(char));
            if (result <= 0) {
                throw EngineException(EngineStatusEnum::TableStructureCorrupted);
            }

            break;
        }
        default:
            throw EngineException(EngineStatusEnum::WrongTableValueType);
    }
}

/*
 * Writes to table file "deleted" flag for row
 * @param tableFD - file descriptor of table
 * @param deleted - "deleted" flag
 * @throws EngineException
 */
void TableIO::writeRowDeletedFlag(int tableFD, bool deleted) throw (EngineException) {
    int result = write(tableFD, &deleted, sizeof(bool));
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
}

/*
 * Deletes file that contains given table
 * @param tableName - table name
 * @throws EngineException
 */
void TableIO::deleteTable(string tableName) throw (EngineException) {
    int result = remove((Config::STORAGE_LOCATION + tableName + Config::TABLE_FILE_EXTENSION).c_str());
    if (result <= 0) {
        throw EngineException(EngineStatusEnum::TableStructureCorrupted);
    }
}