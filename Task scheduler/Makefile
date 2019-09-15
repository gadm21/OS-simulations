GCC      = g++
GCC_FLAGS= -std=c++11 
DEBUG	 = -c -g
SOURCES  = sources
OBJECTS  = objects
INCLUDES = -I headers
BIN      = bin

schedular: PCB.o Schedular.o main.o
	g++ $(GCC_FLAGS) -g PCB.o Schedular.o main.o -o schedular

PCB.o: PCB.cpp
	g++ $(GCC_FLAGS) -c -g PCB.cpp -o PCB.o
Schedualar.o: Schedualar.cpp
	g++ $(GCC_FLAGS) -c -g Schedualar.cpp -o Schedualar.o
main.o: main.cpp
	g++ $(GCC_FLAGS) -c -g main.cpp -o main.o
