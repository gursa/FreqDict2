TARGET = freq

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CC = gcc
RM = rm -f

CFLAGS = -fPIC -Wall -Wpedantic -Wextra -O2

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	@echo "Cleaning..."
	$(RM) $(OBJ) $(TARGET)
