CC = g++
LIBS = -lGL -lGLU -lglut
DEFINES = -DGL_GLEXT_PROTOTYPES

EXE = example
all: keyboard_input.cpp
	$(CC) $^ $(DEFINES) $(LIBS) -o $(EXE)

clean:
	rm $(EXE)

