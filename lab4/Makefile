
all:
	gcc -lmraa -o lab4_1 -lm lab4_uno.c
	gcc -lpthread -lmraa -o lab4_2 -lm lab4_deux.c
	gcc -lssl -lcrypto -lpthread -lmraa -o lab4_3 -lm lab4_tres.c	
	
check:	run_1 run_2 run_3  

clean:
	rm -f *.log *.tar.gz lab4_1 lab4_2 lab4_3

run_1:
	./lab4_1

run_2:
	./lab4_2
	
run_3:
	./lab4_3
	
dist:
	tar -cf lab4-XXXXXX115-zackmeeks.tar.gz lab4_uno.c lab4_deux.c lab4_tres.c *.log README Makefile
	