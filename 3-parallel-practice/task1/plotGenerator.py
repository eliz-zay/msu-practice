import test

def callTests():
    for i in range(0, 6):
        # 1 - write to CSV file
        test.generateAndTest('f', 500, 500, 600, i, 1)

callTests()