.PHONY: all clean
all: init all_inn clean
gen: gen_inn
test: init test_inn clean
init:
	mpic++ -o main main.cpp
	g++ matrixGen.cpp -o gen
gen_inn:
	./gen
all_inn:
	mpirun -np $(arg1) ./main
test_inn:
	mpirun -np 2 ./main
	mpirun -np 4 ./main
	mpirun -np 8 ./main
	mpirun -np 16 ./main
	mpirun -np 20 ./main
clean:
	rm ./main
	rm ./gen