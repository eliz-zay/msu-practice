import sys

import test

def testMatrixes():
    for size in [50, 100, 200, 400, 500, 1000]:
        for mode in range(0, 6):
            for type in ['f', 'd']:
                test.generateAndTest(type, size, size, size, mode, 0)

testMatrixes()