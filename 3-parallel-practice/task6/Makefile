.PHONY: clean
all: init run clean
init:
	mpic++ -std=c++11 src/main.cpp
run:
	mpirun -np $(proc) ./a.out
clean:
	rm -f ./a.out