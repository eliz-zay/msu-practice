import sys
import numpy as np
import struct

EPS = 0.001

def generateAndTest(type, dim1, dim2, dim3, mode, plotFlag):
    fileNameA = "matrixes/A.bin"
    fileNameB = "matrixes/B.bin"
    fileNameRes = "matrixes/Res.bin"

    matrixA = 100 * np.random.rand(dim1, dim2).astype(type)
    matrixB = 100 * np.random.rand(dim2, dim3).astype(type)
    matrixRes = np.dot(matrixA, matrixB).astype(type)

    toFiles(type, dim1, dim2, dim3, mode, matrixA, matrixB, fileNameA, fileNameB)

    runShell("./main " + fileNameA + " " + fileNameB + " " + fileNameRes + " " + str(mode) + " " + str(plotFlag))

    check(type, dim1, dim3, matrixRes, fileNameRes)

def toFiles(type, dim1, dim2, dim3, mode, matrixA, matrixB, fileNameA, fileNameB):
    fileA = open(fileNameA, 'wb')
    fileB = open(fileNameB, 'wb')

    fileA.write(type.encode('ascii'))
    np.array([dim1]).tofile(fileA)
    np.array([dim2]).tofile(fileA)
    np.array(matrixA).tofile(fileA)

    fileB.write(type.encode('ascii'))
    np.array([dim2]).tofile(fileB)
    np.array([dim3]).tofile(fileB)
    np.array(matrixB).tofile(fileB)

    fileA.close()
    fileB.close()

def check(typeExp, rowsExp, colsExp, matrixExp, fileNameRes):
    fileRes = open(fileNameRes, 'rb')
    
    type = fileRes.read(1).decode()
    fileRes.seek(1, 0)
    rows = struct.unpack('i', fileRes.read(4))[0]
    fileRes.seek(4, 1)
    cols = struct.unpack('i', fileRes.read(4))[0]
    fileRes.seek(4, 1)

    dtype = 'float32' if type == 'f' else 'float'
    data = np.fromfile(fileRes, dtype)
    matrix = np.reshape(data, (-1, cols))

    if (
        (type != typeExp) |
        (rows != rowsExp) |
        (cols != colsExp) |
        (not np.allclose(matrix, matrixExp, EPS))
    ):
        print("Wrong result!")
        sys.exit(1)

    fileRes.close()

def runShell(bashCommand):
    import subprocess
    process = subprocess.Popen(bashCommand, shell = True)
    process.communicate()