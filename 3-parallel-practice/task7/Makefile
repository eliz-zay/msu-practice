.PHONY: clean
openmp: init_openmp test_openmp clean
openmp_xlc++: init_openmp_xlc++ test_openmp clean
mpi: init_mpi test_mpi clean
mpi_test: init_mpi test_plot_mpi clean
init_openmp:
	clang++ -Xpreprocessor -fopenmp ./openmp/vOpenMP.cpp -o ./openmp/a.out -lomp
init_openmp_xlc++:
	xlc++ -qsmp=omp -fopenmp ./openmp/vOpenMP.cpp -o ./openmp/a.out
init_mpi:
	mpic++ mpi/vMPI.cpp -o ./mpi/a.out
test_openmp:
	./openmp/a.out
test_mpi:
	mpirun -np $(arg1) ./mpi/a.out $(arg2)
test_plot_mpi:
	mpirun -np 2 ./mpi/a.out 3000
	mpirun -np 4 ./mpi/a.out 3000
	mpirun -np 8 ./mpi/a.out 3000
	mpirun -np 16 ./mpi/a.out 3000
	mpirun -np 20 ./mpi/a.out 3000
clean:
	rm -f ./openmp/a.out
	rm -f ./mpi/a.out