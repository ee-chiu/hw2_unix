.PHONY: all, clean
CXX = g++
CFLAGS = -Wall -g

PROGS = hw2

all: ${PROGS}

hw2: hw2.cpp 
	$(CXX) -o $@ $(CFLAGS) $<

clean:
	rm -f *~ $(PROGS)