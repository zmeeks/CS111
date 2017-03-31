
all:
	gcc -o lab0 -g lab0.c

check:  clean all io_files smoke_tests
	if [ -s letters.txt ]; then \
	echo "smoke test(s) failed"; \
	else \
	echo "all tests passed"; \
	fi
	
clean:
	rm -f *.o lab0 *.tar.gz nada.txt rien.txt input_file.txt 
	rm -f letters.txt out_A.txt out_B.txt out_C.txt out_D.txt out_E.txt

dist: 
	tar -cf lab0-XXXXXX115.tar.gz lab0.c Makefile README *.png 

### smoke tests ###

io_files:
	echo "blah blah blah \n test-file words" > input_file.txt 
	touch nada.txt 
	chmod -w nada.txt 
	touch rien.txt 
	chmod -r rien.txt

A:
	./lab0 --input=input_file.txt --output=out_A.txt ; \
	if [ $$? -ne 0 ]; then \
	echo "Error: exit 0 status not received" >> letters.txt ; \
	fi 
	
B:
	./lab0 --input=input_file.txt > out_B.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: exit 0 status not received" >> letters.txt ; \
	fi 
	
C:
	./lab0 < input_file.txt > out_C.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: exit 0 status not received" >> letters.txt ; \
	fi 
	
D:
	./lab0 < input_file.txt --output=out_D.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: exit 0 status not received" >> letters.txt ; \
	fi 
	
E:
	./lab0 --input=input_file.txt > out_E.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: exit 0 status not received" >> letters.txt ; \
	fi 
	
F:
	./lab0 --input=rien.txt --output=out_F.txt ; \
	if [ $$? -ne 1 ] ; then \
	echo "Error: exit 1 status not received" >> letters.txt ; \
	fi 
	
G:
	./lab0 --input=input_file.txt --output=nada.txt ; \
	if [ $$? -ne 2 ] ; then \
	echo "Error: exit 2 status not received" >> letters.txt ; \
	fi 
	
H:
	./lab0 --catch --segfault ; \
	if [ $$? -ne 3 ] ; then \
	echo "Error: exit 3 status not received" >> letters.txt ; \
	fi 
	
S:	
	./lab0 --segfault ; \
	if [ $$? -ne 139 ] ; then \
	echo "Error: exit 139 (128 + 11) status not received" >> letters.txt ; \
	fi 

Z:
	cmp input_file.txt out_A.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: output file differs from input file" >> letters.txt ; \
	fi ; \
	cmp input_file.txt out_B.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: output file differs from input file" >> letters.txt ; \
	fi ; \
	cmp input_file.txt out_C.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: output file differs from input file" >> letters.txt ; \
	fi ; \
	cmp input_file.txt out_D.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: output file differs from input file" >> letters.txt ; \
	fi ; \
	cmp input_file.txt out_E.txt ; \
	if [ $$? -ne 0 ] ; then \
	echo "Error: output file differs from input file" >> letters.txt ; \
	fi 



smoke_tests:  A B C D E F G H S Z