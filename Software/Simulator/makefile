in := ./src
out := "./bin"
CC = clang++

all: $(in)/SAFETy.o $(in)/Registers.o $(in)/Memory.o $(in)/Monitor.o $(in)/ALU.o $(in)/Control.o $(in)/tools.o
	-@mkdir bin -p
	$(CC) -o $(out)/SAFETy $(in)/SAFETy.o $(in)/Registers.o $(in)/Memory.o $(in)/ALU.o $(in)/Control.o $(in)/Monitor.o $(in)/tools.o -pthread
.PHONY: clean
clean:
	-@rm -r **/*.o

$(in)/SAFETy.o: $(in)/SAFETy.cpp
	$(CC) -c $(in)/SAFETy.cpp
	@mv SAFETy.o $(in)/SAFETy.o

$(in)/Registers.o: $(in)/Registers.cpp
	$(CC) -c $(in)/Registers.cpp 
	@mv Registers.o $(in)/Registers.o

$(in)/Memory.o: $(in)/Memory.cpp
	$(CC) -c $(in)/Memory.cpp
	@mv Memory.o $(in)/Memory.o

$(in)/Monitor.o: $(in)/Monitor.cpp
	$(CC) -c $(in)/Monitor.cpp
	@mv Monitor.o $(in)/Monitor.o

$(in)/ALU.o: $(in)/ALU.cpp
	$(CC) -c $(in)/ALU.cpp
	@mv ALU.o $(in)/ALU.o

$(in)/Control.o: $(in)/Control.cpp
	$(CC) -c $(in)/Control.cpp
	@mv Control.o $(in)/Control.o

$(in)/tools.o: $(in)/tools.cpp
	$(CC) -c $(in)/tools.cpp
	@mv tools.o $(in)/tools.o

.PHONY: debug
debug: all
	./$(out)/SAFETy -m -e




	

