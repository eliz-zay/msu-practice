import test

def test_matrixes():
    for size in [50, 100, 200, 400]:
        for block_size in [32, 64]:
            for mode in range(1, 3):
                    test.generate_and_test(size, block_size, mode)

test_matrixes()