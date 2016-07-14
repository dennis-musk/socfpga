CFLAGS= -Wall -O2 -g
CC=arm-linux-gnueabihf-gcc

i2c: eeprog.o 24cXX.o
	$(CC) $(CFLAGS) -o i2c eeprog.o 24cXX.o

clean: 
	rm -f i2c 24cXX.o eeprog.o

