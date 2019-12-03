APPNAME=$(shell basename `pwd`)

all: $(APPNAME)

%: %.c
	cc -g -Wall -Wextra $^ -o $@ -pthread

.PHONY: clean
clean:
	rm -f $(APPNAME) *.o
