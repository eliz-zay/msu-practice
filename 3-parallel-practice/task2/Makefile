.PHONY: all clean
all: init report_data clean
test: init test_inner clean
init:
	mkdir -p tests
	mkdir -p perf
	g++ main.cpp /usr/lib64/libpapi.so
report_data:
	python test.py
	gnuplot plot_generator
test_inner:
	python test_wrapper.py
clean:
	rm -rf tests
	rm a.out