CC = g++
LIBS = -lGL -lGLU -lglut
DEFINES = -DGL_GLEXT_PROTOTYPES

EXE = vbo_example
all: VBO

clean:
	rm $(EXE)

VBO: vbo.cpp
	$(CC) $^ $(DEFINES) $(LIBS) -o $(EXE)
