CC = mpicc

OBJ = group.o

all: $(OBJ)
	$(CC) $(OBJ) -o group

clean:
	rm $(OBJ) group

%.o: %.c
	$(CC) $^ -c
