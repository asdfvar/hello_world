OBJS = main.o

all: $(OBJS)
	g++ $^ -o main

clean:
	rm -fv $(OBJS) main

%.o: %.cpp
	g++ -march=native $^ -c
