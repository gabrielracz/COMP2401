

# I/O tutotrial makefile
#

person: main_bin.o person.o mystat.o
	gcc -o person main_bin.o person.o mystat.o

main_bin.o: main_bin.c person.h mystat.h
	gcc -c main_bin.c

mystat.o: mystat.h mystat.c
	gcc -c mystat.c

person.o: person.h person.c
	gcc -c person.c

clean: 
	rm -f *.o
	cp emp_original.bin emp.bin


