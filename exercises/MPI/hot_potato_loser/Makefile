APPNAME=$(shell basename `pwd`)

CC=mpicc
CXX=mpic++
FLAGS=-g -fopenmp -Wall -Wextra -std=gnu11 -lm
CFLAGS=$(FLAGS)
CXXFLAGS=$(FLAGS)
LIBS=
all: $(APPNAME)

%: %.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f $(APPNAME) *.o

.PHONY: gitignore
gitignore:
		echo $(APPNAME) > .gitignore
