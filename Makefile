.PHONY: clean, mrproper

IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
CFLAGS = -g -Wall -std=c99 -I$(IDIR)

all: sequential parallel

sequential: $(SDIR)/sequential.c
	$(CC) -o $@ $< $(SDIR)/bubblesort.c $(CFLAGS)

prallel: $(SDIR)/parallel.c
	$(CC) -o $@ $< $(SDIR)/bubblesort.c $(CFLAGS)

clean:
	rm -f *.o core.*

mrproper: clean
	rm -f sequential
	rm -f parallel
