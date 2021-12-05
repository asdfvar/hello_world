all: fmadd256

fmadd256: fmadd256.o
	g++ $^ -o fmadd256

clean:
	rm -fv *.o fmadd256

%.o: %.cpp
	g++ -march=native $^ -c
