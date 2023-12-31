CC=gcc
CFLAGS=-g -Wall -std=gnu11
LDFLAGS=-lwiringPi -li2c

.PHONY: clean

all: simple_radio ptt_on

simple_radio: sbitx_i2c.o sbitx_core.o sbitx_gpio.o sbitx_si5351.o simple_radio.o
	$(CC) -o simple_radio sbitx_i2c.o sbitx_core.o sbitx_gpio.o sbitx_si5351.o simple_radio.o $(LDFLAGS)

ptt_on: sbitx_i2c.o sbitx_core.o sbitx_gpio.o sbitx_si5351.o ptt_on.o
	$(CC) -o ptt_on sbitx_i2c.o sbitx_core.o sbitx_gpio.o sbitx_si5351.o ptt_on.o $(LDFLAGS)


ptt_on.o: ptt_on.c
	$(CC) -c $(CFLAGS) ptt_on.c -o ptt_on.o

simple_radio.o: simple_radio.c
	$(CC) -c $(CFLAGS) simple_radio.c -o simple_radio.o


sbitx_gpio.o: sbitx_gpio.c sbitx_gpio.h
	$(CC) -c $(CFLAGS) sbitx_gpio.c -o sbitx_gpio.o

sbitx_i2c.o: sbitx_i2c.c sbitx_i2c.h
	$(CC) -c $(CFLAGS) sbitx_i2c.c -o sbitx_i2c.o

sbitx_core.o: sbitx_core.c sbitx_core.h
	$(CC) -c $(CFLAGS) sbitx_core.c -o sbitx_core.o

sbitx_si5351.o: sbitx_si5351.c sbitx_si5351.h
	$(CC) -c $(CFLAGS) sbitx_si5351.c -o sbitx_si5351.o



clean:
	rm -f simple_radio ptt_on *.o
