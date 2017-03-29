
all:
	gcc -o lab3a lab3a.c
	
check: 
	./lab3a disk-image 

clean:
	rm -f *.csv *.tar.gz lab3a

easy:
	chmod +rw *.csv
	
dist:
	tar -cf lab3a-XXXXXX115-zackmeeks.tar.gz lab3a.c README Makefile
	