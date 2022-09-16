.PHONY: all clean local polus report check local_init polus_init check_inn report test
local: local_init test clean
polus: polus_init test clean
report: polus_init report_run clean
check: local_init check_inn clean
local_init:
	mpic++ -o main main.cpp -w
polus_init:
	mpicxx -o main main.cpp -w
test:
	mpirun -np $(proc_num) ./main $(n) $(read) $(test) no $(read_file)
check_inn:
	mpirun -np $(proc_num) ./main 2 no no both
report_run:
	rm -f ./time.csv
	touch time.csv
	python test.py
clean:
	rm ./main
