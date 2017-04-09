
build:
	gcc -o lab2_list -g lab2_list.c SortedList.c -lpthread
	gcc -o lab2b_list_mutex_wait lab2b_list_mutex_wait.c SortedList.c -lpthread

tests:	all smoke_test

graphs:	graph_1 graph_2 graph_3 graph_4_5

profile:
	LD_PRELOAD=./lib/libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.gperf
	pprof --list=wrapper_s ./lab2b_list ./raw.gperf >> profile.gperf

clean:
	rm lab2b_list_mutex_wait lab2_list lab2b*.csv profile.gperf lab2b_*.png lab2b*tar.gz lab_2b_list.csv raw.gperf

graph_1:
	gnuplot lab2b_1.gp

graph_2:
	touch lab2b_list_mutex.csv
	rm lab2b_list_mutex.csv
	./lab2b_2_tests.sh
	gnuplot lab2b_2.gp

graph_3:
	touch lab2b_list_3.csv
	rm lab2b_list_3.csv
	./lab2b_3_tests.sh
	gnuplot lab2b_3.gp

graph_4_5:
	touch lab2b_list_4.csv
	rm lab2b_list_4.csv
	./lab2b_4_tests.sh
	gnuplot lab2b_4.gp
	gnuplot lab2b_5.gp

tarball:
	tar -cf lab2b-XXXXXX115-zackmeeks.tar.gz lab2*.c lab2*.csv lab_2*.csv lab2*.png lab2*.gp lab2*.sh SortedList.c SortedList.h Makefile README test*.sh profile.gperf

smoke_test:
	touch lab_2b_list.csv
	rm lab_2b_list.csv
	./test_script_2b.sh
