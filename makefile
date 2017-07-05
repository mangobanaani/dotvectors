CC=gcc
  all: dotvec

  dotvec:  
	gcc -g -o -ansi -pedantic -Wall dotvec main.o pointdata.o
  pointdata.o:
	gcc -g -c pointdata.c
  main.o:
	gcc -g -c main.c

