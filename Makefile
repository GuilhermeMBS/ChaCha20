CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99
TARGET = chacha20

all: $(TARGET)

$(TARGET): chacha20.o operations.o serialization.o
	$(CC) $(CFLAGS) -o $(TARGET) chacha20.o operations.o serialization.o

chacha20.o: chacha20.c operations.h serialization.h
	$(CC) $(CFLAGS) -c chacha20.c

operations.o: operations.c operations.h
	$(CC) $(CFLAGS) -c operations.c

serialization.o: serialization.c serialization.h
	$(CC) $(CFLAGS) -c serialization.c

clean:
	rm -f *.o $(TARGET) *.bin