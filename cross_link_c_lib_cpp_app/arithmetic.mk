AR = ar rcs

CC = gcc

OBJS = arithmetic.o

all: $(OBJS)
	$(AR) libarithmetic.a $^

clean:
	rm $(OBJS) libarithmetic.a

%.o: %.c
	$(CC) $^ -c
