.PHONY: all, clean
CXX = g++
CFLAGS = -Wall -g
CGG = gcc
SO_FLAGS = -shared -fPIC

PROGS = logger logger.so

all: ${PROGS}

logger: hw2.cpp std_map.h 
	$(CXX) -o $@ $(CFLAGS) $<

logger.so: hw2_so.c helper.h
	$(CGG) -o $@ $(SO_FLAGS) $< -ldl

clean:
	rm -f *~ $(PROGS)