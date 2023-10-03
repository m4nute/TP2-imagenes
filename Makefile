.PHONY: all clean

BIN = main

CPP = main.cpp filters.cpp ppm.cpp aplicar.cpp 
OBJ = $(CPP:.cpp=.o)
SRC = $(CPP)

all: main

clean:
	rm -f $(BIN) $(OBJ)

main: $(OBJ)
	g++ -pthread -o $@ $^

%.o: %.cpp
	g++ -pthread -c -o $@ $<
