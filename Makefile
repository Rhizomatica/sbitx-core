CC=gcc
CFLAGS=-g -Wall -std=gnu11
LDFLAGS=-lwiringPi -li2c

all: simple_radio

simple_radio: sbitx_i2c.o sbitx_core.o simple_radio.o
	$(CC) -o simple_radio sbitx_i2c.o sbitx_core.o simple_radio.o $(LDFLAGS)

sbitx_i2c.o: sbitx_i2c.c sbitx_i2c.h
	$(CC) -c $(CFLAGS) sbitx_i2c.c -o sbitx_i2c.o

sbitx_core.o: sbitx_core.c sbitx_core.h
	$(CC) -c $(CFLAGS) sbitx_core.c -o sbitx_core.o

simple_radio.o: simple_radio.c
	$(CC) -c $(CFLAGS) simple_radio.c -o simple_radio.o
