MPI_CC  = $(HOME)/user_libs/openmpi/bin/mpicc
MPI_INC = -I/$(HOME)/user_libs/openmpi/include

all:
	$(MPI_CC) main.c $(MPI_INC) -o main

clean:
	rm -v main
