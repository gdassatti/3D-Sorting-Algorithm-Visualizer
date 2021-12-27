# Assumes the following file exists in the proper place.
include /home/cs425/code-egs/Makefile.defs

# Next line says what options to use to compile programs
OPTIONS+=-Wall -std=c++14

OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc))

all: visualizer

cube.o: cube.h cube.cc object.o
	$(CC) cube.cc -c $(OPTIONS)

object.o: object.cc object.h
	$(CC) object.cc -c $(OPTIONS)

light.o: light.cc light.h
	$(CC) light.cc -c $(OPTIONS)

visualizer: cube.o cube.h object.o object.h light.o light.h visualizer.cc
	$(CC)  visualizer.cc cube.o object.o $(InitShader) $(OPTIONS) $(LDLIBS) -o visualizer

clean:
	rm -f $(OBJECTS) *.o *~