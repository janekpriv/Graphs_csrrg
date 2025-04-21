CC = gcc
CFLAGS = -g -Wall -O0 -I./lib 
TARGET = program
OBJ = graph.o parse_csr.o main.o louvian.o 

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
test: test.o graph.o parse_csr.o louvian.o 
	$(CC) $(CFLAGS) $^ -o test

test.o: src/test.c
	$(CC) $(CFLAGS) -c $< -o $@

parse_csr.o: src/parse_csr.c
	$(CC) $(CFLAGS) -c $< -o $@

louvian.o: src/louvian.c
	$(CC) $(CFLAGS) -c $< -o $@

graph.o: src/graph.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
	del /Q *.o a.out program.exe 2>nul || exit 0