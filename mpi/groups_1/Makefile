CC = mpicc

OBJ0 = group0.o
OBJ1 = group1.o
OBJ2 = group2.o

EXE0 = group0
EXE1 = group1
EXE2 = group2

all: $(OBJ0) $(OBJ1) $(OBJ2)
	$(CC) $(OBJ0) -o $(EXE0)
	$(CC) $(OBJ1) -o $(EXE1)
	$(CC) $(OBJ2) -o $(EXE2)

clean:
	rm $(OBJ0) $(OBJ1) $(OBJ2) $(EXE0) $(EXE1) $(EXE2)

%.o: %.c
	$(CC) $^ -c
