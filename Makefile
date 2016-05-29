ENVLAD := $(shell command -v ladcomp 2> /dev/null)

.PHONY: clean, mrproper

IDIR = ./include
SDIR = ./src
ODIR = ./obj

MPI = mpicc
LAD = ladcomp

LADFLAGS = -env mpicc

CC = gcc
CFLAGS = -g -Wall -std=c99 -I$(IDIR)

_DEPS = bubblesort.c utils.c
DEPS = $(patsubst %,$(SDIR)/%,$(_DEPS))

all: sequential parallel

sequential: $(SDIR)/sequential.c
	$(CC) -o $@ $< $(SDIR)/bubblesort.c $(CFLAGS)

parallel: $(SDIR)/parallel.c
ifndef $(ENVLAD)
	$(MPI) -o $@ $< $(DEPS) $(CFLAGS)
else
	$(LAD) $(LADFLAGS) -o $@ $< $(DEPS) $(CFLAGS)
endif

clean:
	rm -f *.o core.*

mrproper: clean
	rm -f sequential
	rm -f parallel
