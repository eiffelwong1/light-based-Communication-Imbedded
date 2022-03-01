all: sender receiver

sender:
	gcc -o sender -lwiringPi hamming.h sender.c

receiver:
	gcc -o receiver -lwiringPi hamming.h receiver.c

flash:
	gcc -o flash -lwiringPi flash.c

hamming:
	gcc -o hamming hamming.c

clean:
	rm sender receiver
