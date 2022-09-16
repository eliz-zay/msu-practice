import subprocess
import random


def gen_mapfile():
    coords = []
    for x in range(0, 4):
        for y in range(0, 8):
            for z in range(0, 8):
                for t in range(0, 2):
                    coords.append(str(x) + " " + str(y) + " " + str(z) + " " + str(t))

    random.shuffle(coords)

    file = open('my.map', 'w')
    for i in range(0, 512):
        file.write(coords[i])


def run_process(command):
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    process.communicate()


def create_csv(rows, cols):
    for process_pow in range(5, 10):
        run_process("python matrixGen.cpp " + str(rows) + " " + str(cols))
        if rows == 512 and cols == 512:
            run_process("mpisubmit.bg -n " + 2**process_pow + "-w 00:05:00 --stdout " + str(rows) + "_" + str(cols) + "_random.csv ./main.out -mapfile my.map")

        run_process("mpisubmit.bg -n " + 2**process_pow + "-w 00:05:00 --stdout " + str(rows) + "_" + str(cols) + ".csv ./main.out")


gen_mapfile()

create_csv(512, 512)
create_csv(1024, 1024)
create_csv(2048, 2048)
create_csv(4096, 4096)
create_csv(4096, 1024)
create_csv(1024, 4096)