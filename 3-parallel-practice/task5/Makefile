.PHONY: all gen test init gen_inn all_inn test_inn clean
all: init all_inn clean
gen: init gen_inn clean
test: init test_inn clean
init:
	mpic++ -o main main.cpp
	g++ gen.cpp -o gen
	g++ test.cpp -o test
gen_inn:
	./gen $(arg1)
all_inn:
	mpirun -np $(arg1) ./main $(arg2) $(arg3) $(arg4)
test_inn:
	./test $(arg1) $(arg2)
clean:
	rm -f ./main
	rm -f ./gen
	rm -f ./test