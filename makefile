# makefile for MP5
# Christian Walker
#

comp = gcc
comp_flags = -g -Wall
comp_libs = -lm
#comp = cc
lab5: list.o lab5.o
	gcc -Wall -g list.o lab5.o -o lab5 -lm

list.o : list.c bst.h
	 gcc -Wall -g -c list.c

lab5.o : lab5.c bst.h
	 gcc -Wall -g -c lab5.c

clean :
	 rm -f *.o lab5 core
