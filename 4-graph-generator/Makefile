.PHONY: all init run clean lint
all: init run clean
init:
	clang++ src/*.cpp -o main -std=c++17 -pthread -Werror
run:
	./main
clean:
	rm -f ./main
lint:
	clang-format -i -style=Chromium src/*.cpp src/*.hpp
