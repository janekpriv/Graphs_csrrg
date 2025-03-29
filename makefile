CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = program
OBJ = graph.o parse_csr.o main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q *.o a.out program.exe 2>nul || exit 0