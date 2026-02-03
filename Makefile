# Compiler and Flags
CC = gcc
# -Isrc tells the compiler to look for header files in the 'src' folder
CFLAGS = -Wall -Wextra -g -Isrc

# The final executable name
TARGET = chacha20

# Object files 
# We explicitly tell Make that chacha.o lives inside src/
OBJS = main.o src/chacha.o

# Default rule
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule for main.o (Depends on main.c AND the header in src/)
main.o: main.c src/chacha.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Rule for src/chacha.o (Depends on the files inside src/)
src/chacha.o: src/chacha.c src/chacha.h
	$(CC) $(CFLAGS) -c src/chacha.c -o src/chacha.o

# Clean rule (Updated for Windows/MinGW)
clean:
	del main.o src\chacha.o $(TARGET).exe 2>NUL