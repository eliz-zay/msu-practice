def runShell(bashCommand):
    import subprocess
    process = subprocess.Popen(bashCommand, shell = True)
    process.communicate()

for q in range(3):
    for nqubits in [20, 24, 28, 30]:
        qNum = 0
        if q == 0:
            qNum = 5
        elif q == 1:
            qNum = 1
        elif q == 2:
            qNum = nqubits

        for thread in [1, 2, 4, 8, 64]:
            bashCommand = "./main " + str(qNum) + " " +  str(nqubits) + " " + str(thread)
            runShell(bashCommand)