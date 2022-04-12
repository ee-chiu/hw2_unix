.PHONY: all, clean
CXX = g++
CFLAGS = -Wall -g
CGG = gcc
SO_FLAGS = -shared -fPIC

PROGS = hw2 hw2.so

all: ${PROGS}

hw2: hw2.cpp 
	$(CXX) -o $@ $(CFLAGS) $<

hw2.so: hw2_so.c
	$(CGG) -o $@ $(SO_FLAGS) $< -ldl

clean:
	rm -f *~ $(PROGS)