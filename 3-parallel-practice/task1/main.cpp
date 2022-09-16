#include <iostream>
#include <ios>
#include <fstream>
#include <exception>
#include <cmath>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "plot.h"

using namespace std;


struct MatrixWrapper {
    void** matrix;
    char type;
    size_t rows, cols;
    
    MatrixWrapper(): type(' '), rows(0), cols(0), matrix(NULL) {}

    MatrixWrapper(char type, size_t rows, size_t cols, void** matrix): type(type), rows(rows), cols(cols), matrix(matrix) {}

    void print() {
        for (size_t i = 0; i < rows; ++i) {
                cout << endl;
                for (size_t j = 0; j < cols; ++j) {
                    if (type == 'f') {
                        cout << reinterpret_cast<float**>(matrix)[i][j] << " ";
                    } else if (type == 'd') {
                        cout << reinterpret_cast<double**>(matrix)[i][j] << " ";
                    }
                }
        }
        cout << endl;
    }
    
    void toFile(const char* fileName) {
        int fd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, 0777);

        write(fd, &(this->type), sizeof(char));
        write(fd, &(this->rows), sizeof(size_t));
        write(fd, &(this->cols), sizeof(size_t));

        size_t typeSize = this->type == 'f' ? sizeof(float) : sizeof(double);

        for (size_t i = 0; i < this->rows; i++) {
            write(fd, this->matrix[i], typeSize * (this->cols));
        }

        close(fd);
    }

    static MatrixWrapper fromFile(const char* fileName) {
        char type;
        size_t rows, cols;

        int fd = open(fileName, O_RDONLY, 0777);

        read(fd, &type, sizeof(char));
        read(fd, &rows, sizeof(size_t));
        read(fd, &cols, sizeof(size_t));

        size_t typeSize = type == 'f' ? sizeof(float) : sizeof(double);

        void** matrix = new void*[rows];

        for (size_t i = 0; i < rows; i++) {
            if (type == 'f') {
                matrix[i] = new float[cols];
            } else {
                matrix[i] = new double[cols];
            }
            read(fd, matrix[i], typeSize * cols);
        }

        close(fd);

        return MatrixWrapper(type, rows, cols, (void**)matrix);
    }
    
    template <typename T>
    void multiply(clock_t &time, MatrixWrapper &resultWrapper, int mode, MatrixWrapper matrixWrapper) {
        time = clock();
        if (this->cols != matrixWrapper.rows) {
            throw logic_error("Incompatible matrix sizes");
        }
        
        if (this->type != matrixWrapper.type) {
            throw logic_error("Diffirent matrix types");
        }
        
        T** resMatrix = new T* [this->rows];
        for (size_t i = 0; i < this->rows; ++i) {
            resMatrix[i] = new T [matrixWrapper.cols];
        }
        
        MatrixWrapper result(this->type, this->rows, matrixWrapper.cols, (void**)resMatrix);
        
        switch (mode) {
            case 0: { //ijk
                for (size_t i = 0; i < this->rows; ++i) {
                    for (size_t j = 0; j < matrixWrapper.cols; ++j) {
                        T sum = 0.;
                        for (size_t k = 0; k < this->cols; ++k) {
                            sum += reinterpret_cast<T**>(this->matrix)[i][k] * reinterpret_cast<T**>(matrixWrapper.matrix)[k][j];
                        }
                        resMatrix[i][j] = sum;
                    }
                }
                break;
            }
            case 1: { //ikj
                for (size_t i = 0; i < this->rows; ++i) {
                    for (size_t k = 0; k < this->cols; ++k) {
                        T elem = reinterpret_cast<T**>(this->matrix)[i][k];
                        for (size_t j = 0; j < matrixWrapper.cols; ++j) {
                            resMatrix[i][j] += elem * reinterpret_cast<T**>(matrixWrapper.matrix)[k][j];
                        }
                    }
                }
                break;
            }
            case 2: { //kij
                for (size_t k = 0; k < this->cols; ++k) {
                    for (size_t i = 0; i < this->rows; ++i) {
                        T elem = reinterpret_cast<T**>(this->matrix)[i][k];
                        for (size_t j = 0; j < matrixWrapper.cols; ++j) {
                            resMatrix[i][j] += elem * reinterpret_cast<T**>(matrixWrapper.matrix)[k][j];
                        }
                    }
                }
                break;
            }
            case 3: { //jik
                for (size_t j = 0; j < matrixWrapper.cols; ++j) {
                    for (size_t i = 0; i < this->rows; ++i) {
                        T sum = 0.;
                        for (size_t k = 0; k < this->cols; ++k) {
                            sum += reinterpret_cast<T**>(this->matrix)[i][k] * reinterpret_cast<T**>(matrixWrapper.matrix)[k][j];
                        }
                        resMatrix[i][j] = sum;
                    }
                }
                break;
            }
            case 4: { //jki
                for (size_t j = 0; j < matrixWrapper.cols; ++j) {
                    for (size_t k = 0; k < this->cols; ++k) {
                        T elem = reinterpret_cast<T**>(matrixWrapper.matrix)[k][j];
                        for (size_t i = 0; i < this->rows; ++i) {
                            resMatrix[i][j] += elem * reinterpret_cast<T**>(this->matrix)[i][k];
                        }
                    }
                }
                break;
            }
            case 5: { //kji
                for (size_t k = 0; k < this->cols; ++k) {
                    for (size_t j = 0; j < matrixWrapper.cols; ++j) {
                        T elem = reinterpret_cast<T**>(matrixWrapper.matrix)[k][j];
                        for (size_t i = 0; i < this->rows; ++i) {
                            resMatrix[i][j] += elem * reinterpret_cast<T**>(this->matrix)[i][k];
                        }
                    }
                }
                break;
            }
        }
        
        resultWrapper = result;
        time = clock() - time;
    }
};

void parseCommand(int argc, char** argv, string& fileName1, string& fileName2, string& fileName3, int& mode, int& plotFlag) {
    if (argc != 6 || atoi(argv[4]) < 0 || atoi(argv[4]) > 5) {
        throw invalid_argument("Wrong parameters!");
    }

    fileName1 = argv[1];
    fileName2 = argv[2];
    fileName3 = argv[3];
    mode = atoi(argv[4]);
    plotFlag = atoi(argv[5]);
}

int main(int argc, char** argv) {
    try {
        string fileA, fileB, fileRes;
        int mode, plotFlag;
        clock_t time = 0;
        float seconds;

        parseCommand(argc, argv, fileA, fileB, fileRes, mode, plotFlag);

        MatrixWrapper matrixA = MatrixWrapper::fromFile(fileA.c_str());
        MatrixWrapper matrixB = MatrixWrapper::fromFile(fileB.c_str());
        MatrixWrapper matrixRes;

        if (matrixA.type == 'd') {
            matrixA.multiply<double>(time, matrixRes, mode, matrixB);
        } else  if (matrixA.type == 'f') {
            matrixA.multiply<float>(time, matrixRes, mode, matrixB);
        }

        matrixRes.toFile(fileRes.c_str());
        seconds = ((float)time)/CLOCKS_PER_SEC;
        cout << "Mode: " << mode << endl;
        cout << "Type: " << matrixRes.type << endl;
        cout << "Size: " << matrixRes.rows << " x " << matrixRes.cols << endl;
        cout << "Time: " << seconds << " sec" << endl;
        cout << endl;

        if (plotFlag) {
            writeToCSV("data.csv", mode, seconds);
        }
    } catch (exception &err) {
        cout << err.what() << endl;
    }
}
