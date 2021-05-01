# GBS Shell Makefile

CC = gcc
CFLAGS  = -Wall -g
OBJ = build.o Utility.o gbsh.o
TARGET = gbsh

all: $(TARGET)

gbsh: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) *~
