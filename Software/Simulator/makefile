in := ./src
out := "./bin"
CC = g++

all: SAFETy.o Registers.o
	-@mkdir bin
	$(CC) -o $(out)/SAFETy ./SAFETy.o Registers.o

	rm *.o

SAFETy.o: $(in)/SAFETy.cpp
	$(CC) -c $(in)/SAFETy.cpp

Registers.o: $(in)/Registers.cpp
	$(CC) -c $(in)/Registers.cpp 





	

