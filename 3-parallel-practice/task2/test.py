import sys
import numpy as np
import math
import subprocess

EPS = 0.001

tests_dir = './tests'

def generate_and_test(size, block_size, mode):
    matrix_A = generate_matrix(size, size)
    matrix_B = generate_matrix(size, size)
    matrix_C = np.dot(matrix_A, matrix_B)

    to_bin(matrix_A, size, size, str(size) + '_A')
    to_bin(matrix_B, size, size, str(size) + '_B')
    to_bin(matrix_C, size, size, str(size) + '_C_expected')

    prog = "./a.out"
    command = f"{prog} {tests_dir}/{size}_A {tests_dir}/{size}_B {tests_dir}/{size}_C {block_size} {mode}"
    run_shell(command)

    matrix_res = from_bin(f"{tests_dir}/{size}_C")

    if not are_equal(matrix_C, matrix_res):
        print("Wrong result of multiplication")
        sys.exit()


def to_bin(matrix, rows, cols, file_name):
    file = open(tests_dir + "/" + file_name, 'wb')

    np.array([rows]).tofile(file)
    np.array([cols]).tofile(file)
    np.array(matrix).tofile(file)

    file.close()


def from_bin(file_name):
    file = open(file_name, 'rb')

    rows = int.from_bytes(file.read(4), 'little')
    file.seek(4, 1)

    cols = int.from_bytes(file.read(4), 'little')
    file.seek(4, 1)

    matrix_bin = np.fromfile(file, dtype = 'float32')
    matrix = np.reshape(matrix_bin, (-1, cols))

    file.close()

    return matrix


def are_equal(matrix_A, matrix_B):
    return np.allclose(matrix_A, matrix_B, EPS)


def generate_matrix(rows, cols):
    return 100 * np.random.rand(rows, cols).astype('float32')


def run_shell(command):
    process = subprocess.run(command.split(' '), stdout = subprocess.PIPE)
    return process.stdout.decode()


def get_perfomance(command):
    output = run_shell(command)

    strings = output.split('\n')
    time = strings[3].split(' ')[-2]
    l1_misses = strings[4].split(' ')[-1]
    l2_misses = strings[5].split(' ')[-1]
    cycles = strings[6].split(' ')[-1]
    tlb = strings[7].split(' ')[-1]

    return (time, l1_misses, l2_misses, cycles, tlb)


if __name__ == '__main__':
    
    time_file = open('./perf/time.csv', 'w')
    l1_file = open('./perf/l1_misses.csv', 'w')
    l2_file = open('./perf/l2_misses.csv', 'w')
    cycles_file = open('./perf/cycles.csv', 'w')
    tlb_file = open('./perf/tlb.csv', 'w')

    for i in range(1, 6): # test on sizes 1000..5000
        factor = 1000
        size = i * factor

        matrix_A = generate_matrix(size, size)
        matrix_B = generate_matrix(size, size)
        matrix_C = np.dot(matrix_A, matrix_B)

        to_bin(matrix_A, size, size, str(size) + '_A')
        to_bin(matrix_B, size, size, str(size) + '_B')
        to_bin(matrix_C, size, size, str(size) + '_C_expected')

        prog = './a.out'
        command1 = f"{prog} {tests_dir}/{size}_A {tests_dir}/{size}_B {tests_dir}/{size}_C 32 1"
        command2 = f"{prog} {tests_dir}/{size}_A {tests_dir}/{size}_B {tests_dir}/{size}_C 32 2"
        command3 = f"{prog} {tests_dir}/{size}_A {tests_dir}/{size}_B {tests_dir}/{size}_C 52 2"

        perf1 = get_perfomance(command1)
        perf2 = get_perfomance(command2)
        perf3 = get_perfomance(command3)

        time_file.writelines(f"{i} {perf1[0]} {perf2[0]} {perf3[0]}\n")
        l1_file.writelines(f"{i} {perf1[1]} {perf2[1]} {perf3[1]}\n")
        l2_file.writelines(f"{i} {perf1[2]} {perf2[2]} {perf3[2]}\n")
        cycles_file.writelines(f"{i} {perf1[3]} {perf2[3]} {perf3[3]}\n")
        tlb_file.writelines(f"{i} {perf1[4]} {perf2[4]} {perf3[4]}\n")

    time_file.close()
    l1_file.close()
    l2_file.close()
    cycles_file.close()