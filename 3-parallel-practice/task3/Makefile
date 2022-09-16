.PHONY: all clean
all: init all_inn clean
test: init test_inn clean
init:
	rm -f max.csv
	rm -f total.csv
	mpic++ -o main main.cpp
all_inn:
	mpirun -np $(arg1) ./main $(arg2) $(arg3) $(arg4)
test_inn:
	python test.py
	gnuplot plotMax.p
	gnuplot plotTotal.p
clean:
	rm ./main