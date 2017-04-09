all:
	gcc -o simpsh -g lab1c.c

check:	benchmark

benchmark:  clean all bench_tests
		
clean:
	rm -f *.o simpsh *.tar.gz file*.txt err*.txt
	
dist: 
	tar -cf lab1c-XXXXXX115-zackmeeks.tar.gz lab1c.c Makefile README profile_report.pdf \
		simpsh--profile_runs.pdf bashdash1.sh bashdash2.sh bashdash3.sh

### bench-marking runs ###

s1:
	./simpsh --profile --rdonly a0.txt --trunc --wronly file1.txt --creat --wronly err1.txt \
		--pipe --pipe --pipe --pipe --pipe --command 0 4 2 tr [:blank:] '\n' --command 3 6 2 sort \
		--command 5 8 2 tr 'k' '\n' --command 7 10 2 sort --command 9 12 2 rev --command 11 1 2 wc --wait	
	echo -e "bench test A done \n\n\n\n\n\n\n\n"

s2:
	./simpsh --profile --rdonly a0.txt --trunc --wronly file2.txt --creat --wronly err2.txt \
		--pipe --pipe --pipe --pipe --pipe --command 0 4 2 tr [:blank:] '\n' --command 3 6 2 sort \
		--command 5 8 2 rev --command 7 10 2 shuf --command 9 12 2 rev --command 11 1 2 wc --wait
	echo -e "bench test B done \n\n\n\n\n\n\n\n"	
		
s3:
	./simpsh --profile --rdonly a0.txt --trunc --wronly file3.txt --creat --wronly err3.txt \
		--pipe --pipe --pipe --pipe --pipe --command 0 4 2 tr [:blank:] '\n' --command 3 6 2 sort \
		--command 5 8 2 tr a-d A-D --command 7 10 2 tr C-G k-o --command 9 12 2 sort \
		--command 11 1 2 rev --wait
	echo -e "bench test C done \n\n\n\n\n\n\n\n"
	
	
bench_tests: s1 s2 s3 