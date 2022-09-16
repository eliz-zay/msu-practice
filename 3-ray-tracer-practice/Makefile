.PHONY: all mac linux init_mac init_linux run clean
all: init_mac run clean
mac: init_mac run clean
linux: init_linux run clean
init_mac:
	clang++ -Xpreprocessor -fopenmp -std=c++11 src/main.cpp -o ./raytracer -lomp -w
init_linux:
	g++ -fopenmp src/main.cpp -o ./raytracer
run:
	./raytracer -w $(size)
clean:
	rm -f ./raytracer
