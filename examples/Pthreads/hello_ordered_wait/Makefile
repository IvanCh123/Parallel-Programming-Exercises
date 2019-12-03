all: hola

hola: hello.c
	cc -g -Wall -Wextra hello.c -o hola -pthread

.PHONY: clean
clean:
	rm -f hola *.o
