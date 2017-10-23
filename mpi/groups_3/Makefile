CC = mpicc

OBJ0 = group0.o
OBJ1 = group1.o

EXE0 = group0
EXE1 = group1

all: $(OBJ0) $(OBJ1) $(OBJ2)
	$(CC) $(OBJ0) -o $(EXE0)
	$(CC) $(OBJ1) -o $(EXE1)

clean:
	rm $(OBJ0) $(OBJ1) $(OBJ2) $(EXE0) $(EXE1)

%.o: %.c
	$(CC) $^ -c
