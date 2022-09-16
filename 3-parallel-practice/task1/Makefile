.PHONY: all
all: init
	g++ main.cpp -o main
init:
	mkdir matrixes
clean:
	rm -f -r matrixes
	rm -f main
	rm -f data.csv
test: init
	g++ main.cpp -o main
	python testWrapper.py
report: init
	rm -f data.csv
	g++ main.cpp -o main
	python plotGenerator.py
	gnuplot force.p