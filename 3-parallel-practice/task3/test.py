def runShell(bashCommand):
    import subprocess
    process = subprocess.Popen(bashCommand, shell = True)
    process.communicate()

for proc in [1,2,4,8,16,20]:
    bashCommand = f"mpirun -np {proc} ./main 1 1000"
    runShell(bashCommand)