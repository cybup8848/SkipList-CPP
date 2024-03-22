CC = g++
CXXFLAGS = -std=c++0x
CFLAGS=-I
DIR=bin
skiplist:main.o
	$(CC) -o ./bin/main main.o --std=c++11 -pthread -fpermissive
	rm -rf ./*.o
main.o:main.cpp
	$(CC) -c $^ -o $@

clean:
	rm -f ./*.o











