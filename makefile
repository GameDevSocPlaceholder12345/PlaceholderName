CC=clang++
CFLAGS=-c -O3 --std=c++14 -g -Wall -iquote include/ -iquote depend/include/
LDFLAGS=-lSDL2 -lSDL2_mixer -pthread
SDIR=src/
#SOURCES=$(wildcard ./$(SDIR)*.cpp)
SOURCES=$(shell find $(SDIR) -type f -name '*.cpp')
CSOURCES=$(shell find $(SDIR) -type f -name '*.c')
OBJECTS=$(SOURCES:.cpp=.o) $(CSOURCES:.c=.o)

EXECUTABLE=PlaceholderName

all: $(SOURCES) $(CSOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean: 
	rm -f src/*.o
	rm -f $(OBJECTS)

hardClean:
	rm -f src/*.o
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
