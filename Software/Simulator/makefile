in := ./src
out := "./bin"
CC = g++

all: SAFETy.o Registers.o Memory.o Monitor.o ALU.o Control.o
	-@mkdir bin -p
	$(CC) -o $(out)/SAFETy ./SAFETy.o Registers.o Memory.o ALU.o Control.o Monitor.o

	rm *.o

SAFETy.o: $(in)/SAFETy.cpp
	$(CC) -c $(in)/SAFETy.cpp

Registers.o: $(in)/Registers.cpp
	$(CC) -c $(in)/Registers.cpp 

Memory.o: $(in)/Memory.cpp
	$(CC) -c $(in)/Memory.cpp

Monitor.o: $(in)/Monitor.cpp
	$(CC) -c $(in)/Monitor.cpp

ALU.o: $(in)/ALU.cpp
	$(CC) -c $(in)/ALU.cpp

Control.o: $(in)/Control.cpp
	$(CC) -c $(in)/Control.cpp





	

