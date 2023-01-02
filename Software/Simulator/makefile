in := ./src
out := "./bin"
CC = clang++

all: $(in)/SAFETy.o $(in)/Registers.o $(in)/Memory.o $(in)/Monitor.o $(in)/ALU.o $(in)/Control.o $(in)/tools.o $(in)/Stack.o
	-@mkdir bin -p
	$(CC) -o $(out)/SAFETy $(in)/SAFETy.o $(in)/Registers.o $(in)/Memory.o $(in)/ALU.o $(in)/Control.o $(in)/Monitor.o $(in)/tools.o $(in)/Stack.o -pthread
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

$(in)/Stack.o: $(in)/Stack.cpp
	$(CC) -c $(in)/Stack.cpp
	@mv Stack.o $(in)/Stack.o

.PHONY: debug
debug: all
	./$(out)/SAFETy -m -e -r -d


.PHONY: install
install:
	@echo "Copying SAFETy into /usr/local/bin"
	@cp ./bin/SAFETy /usr/local/bin
	@echo "Creating 'man' file"
	-@mkdir /usr/local/man/man1 >> /dev/null
	-@cp SAFETy.1 /usr/local/man/man1
	-@gzip -f /usr/local/man/man1/SAFETy.1
	-@mandb >> /dev/null

.PHONY: remove
remove:
	@echo "Deleting SAFETy from /usr/local/bin"
	@-rm /usr/local/bin/SAFETy
	@echo "Removing Man File"
	-@rm /usr/local/man/man1/SAFETy.*
	-@mandb >> /dev/null
