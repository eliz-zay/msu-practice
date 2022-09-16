.PHONY: all clean local polus report local_init polus_init report test
local: local_init test clean
polus: polus_init test clean
report: polus_init report_run clean
local_init:
	mpic++ -o main main.cpp -w
polus_init:
	mpicxx -o main main.cpp -w
test:
	mpirun -np $(proc_num) ./main $(q) $(n) $(read) $(test) $(read_file)
report_run:
	rm -f ./time.csv
	touch time.csv
	python test.py
clean:
	rm ./main
