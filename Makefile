CC = g++
LDFLAGS =
CFLAGS = -I include -g -c -std=c++11
SRC = $(wildcard *.cpp src/*.cpp)
HEAD = $(wildcard include/Nitrogen/*.h)
OBJ = $(SRC:.cpp=.o)
EXEC = nlang

all: $(OBJ) $(EXEC) $(HEAD)

debug: CFLAGS+=-DDEBUG -O0
debug: all
	
release: CFLAGS+=-O3
release: all

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o src/*.o $(EXEC) *.nc *.nvm .symdat