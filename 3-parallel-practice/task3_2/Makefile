.PHONY: all clean
all: init
plot: init plot_inner clean
init:
	rm -f total.csv
	g++ -o main main.cpp
plot_inner:
	./main 1 1 10000000 /dev/null
	./main 2 1 10000000 /dev/null
	./main 4 1 10000000 /dev/null
	./main 8 1 10000000 /dev/null
	./main 16 1 10000000 /dev/null
	./main 32 1 10000000 /dev/null
	./main 64 1 10000000 /dev/null
	./main 128 1 10000000 /dev/null
	gnuplot plot_total.p
clean:
	rm ./main
	rm out.txt
	rm total.csv