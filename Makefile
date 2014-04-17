CC = g++
DEBUG = -g

LIBS = -lboost_system\
	   -lboost_filesystem\
	   -lboost_thread\

LDFLAGS = -Wall $(LIBS)

# -Wall displays warning
# -I./ adds the current directory as a root to look for the included files
CFLAGS = -Wall -c -I./src -I./ -std=c++11 -g

SRC=$(shell find . -name '*.cpp')

OBJECTS = $(SRC:.cpp=.o)
EXECUTABLE = tictacgen.app

all: $(SRC) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o ./bin/$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./bin/tictacgen.app

clean:
	@echo "Recursively cleaning all object files"
	find . -name "*.o" -print0 | xargs -0 rm -rfv

cleanall: clean
	@echo "Recursively cleaning all binary files"
	find . -name "*.app" -print0 | xargs -0 rm -rfv

